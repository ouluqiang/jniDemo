
#include "log.h"
#include <string.h>

#ifdef linux
#include <stdatomic.h>
static _Atomic(int) log_file_size = 0;
#endif

#define USE_SHORT_NAME 0
#define LOG_USE_COLOR 0



#ifdef linux
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x)   (x)
#define unlikely(x) (x)
#endif

static struct {
  void *udata;
  log_LockFn lock;
  int level;
  bool quiet;
  int log_to_file;
  char log_path[1024];
  int log_level;
  int max_size;
  int max_save_log;
} L;
FILE *fp = NULL;


static const char *level_strings[] = {
  "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

#if LOG_USE_COLOR
static const char *level_colors[] = {
  "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif

void log_init(){
    memset(&L,0,sizeof(L));
}

static  void stdout_callback(log_Event *ev) {
  char buf[16];
  buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';

#if LOG_USE_COLOR
  fprintf(
    ev->udata, "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
    buf, level_colors[ev->level], level_strings[ev->level],
    ev->file, ev->line);
#else
  fprintf(
    ev->udata, "%s %-5s %s:%d: ",
    buf, level_strings[ev->level], ev->file, ev->line);
#endif
  vfprintf(ev->udata, ev->fmt, ev->ap);
  fprintf(ev->udata, "\n");
  fflush(ev->udata);
}



static void file_callback(log_Event *ev) {
  char buf[64];
  buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ev->time)] = '\0';
  fprintf(
    ev->udata, "%s %-5s %s:%d: ",
    buf, level_strings[ev->level], ev->file, ev->line);
  vfprintf(ev->udata, ev->fmt, ev->ap);
  fprintf(ev->udata, "\n");
  fflush(ev->udata);
}


static void lock(void)   {
  if (L.lock) { L.lock(true, L.udata); }
}


static void unlock(void) {
  if (L.lock) { L.lock(false, L.udata); }
}


const char* log_level_string(int level) {
  return level_strings[level];
}


void log_set_lock(log_LockFn fn, void *udata) {
  L.lock = fn;
  L.udata = udata;
}


void log_set_level(int level) {
  L.level = level;
}


void log_set_quiet(bool enable) {
  L.quiet = enable;
}

int log_config_file(const char *logpath, int level, int max_size_mb){
    strcpy(L.log_path,logpath);
    L.log_level = level;
    if (max_size_mb <= 0){
        max_size_mb = 500;
    }
    L.max_size = 1024*1024*max_size_mb;
    L.log_to_file = 1;
    L.max_save_log = 5;

    fp = fopen(L.log_path,"a+");
    if (!fp){
        return -1;
    }
    return 0;
}
static int file_rotate(void){

    int n, err = 0;
    const int SUFFIX_LEN = 10;
    fclose(fp);
    char oldpath[1024]= {0}, newpath[1024] = {0};
    strcpy(oldpath,L.log_path);
    strcpy(newpath,L.log_path);
    size_t base = strlen(L.log_path);
    FILE *tmp_fp;

    for (n = L.max_save_log;n >= 0; --n){
        snprintf(oldpath + base, SUFFIX_LEN, n ? ".%d" : "", n - 1);
        snprintf(newpath + base, SUFFIX_LEN, ".%d", n);
        if (n == L.max_save_log ){
            if  ((tmp_fp = fopen(newpath , "r")) != NULL){
                fclose(tmp_fp);
                remove(newpath);
            }
        }
        /* change the new log file to old file name */
        if ((tmp_fp = fopen(oldpath , "r")) != NULL) {
            fclose(tmp_fp);
            err = rename(oldpath, newpath);
        }
    }
    fp = fopen(L.log_path,"w+");

    return (!fp);
}

static void init_event(log_Event *ev, void *udata) {
  if (!ev->time) {
    time_t t = time(NULL);
    ev->time = localtime(&t);
  }
  ev->udata = udata;
}


void log_log(int level, const char *file, int line, const char *fmt, ...) {

    // 去掉*可能*存在的目录路径，只保留文件名
#ifdef USE_SHORT_NAME
    const char* tmp = strrchr(file, '/');
    if (!tmp)
        tmp = file;
    else
        tmp++;
#else
    const char* tmp = file;
#endif
  log_Event ev = {
    .fmt   = fmt,
    .file  = tmp,
    .line  = line,
    .level = level,
  };

//  lock();

  if (!L.quiet && level >= L.level) {
    init_event(&ev, stderr);
    va_start(ev.ap, fmt);
    stdout_callback(&ev);
    va_end(ev.ap);
  }
  if (L.log_to_file && level >= L.log_level){
#ifdef linux
      log_file_size += 1;
      if (log_file_size > 100){
          log_file_size = 0;
#endif
          int rotate_ok = 0;
          lock();
          size_t file_size = 0;
          fseek(fp, 0L, SEEK_END);
          file_size = ftell(fp);
          if (unlikely(file_size > L.max_size)){
              rotate_ok = file_rotate();
          }
          unlock();
          if (rotate_ok != 0){
              return;
          }
#ifdef linux
      }
#endif
      init_event(&ev, fp);
      va_start(ev.ap, fmt);
      file_callback(&ev);
      va_end(ev.ap);
  }
//  unlock();
}
