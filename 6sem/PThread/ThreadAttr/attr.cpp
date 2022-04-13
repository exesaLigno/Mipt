#include <pthread.h>
#include <fstream>

void* threadFunction(void* _);
void printAttributes(pthread_attr_t* attributes);

int main()
{
  pthread_attr_t attributes;
  pthread_t thread;

  pthread_attr_init(&attributes);

  pthread_create(&thread, NULL, threadFunction, NULL);
  pthread_join(thread, NULL);

  pthread_attr_destroy(&attributes);
  return 0;
}


void* threadFunction(void* _)
{
  pthread_attr_t attr;
  pthread_getattr_np(pthread_self(), &attr);
  printAttributes(&attr);
  return nullptr;
}

void printAttributes(pthread_attr_t* attributes)
{
  int status = 0;

  void* stackaddr = nullptr;
  size_t stacksize = 0, guardsize = 0;
  int scope = 0, detachstate = 0, inheritsched = 0, schedpolicy = 0;
  sched_param schedule_parameters;
  status = pthread_attr_getstack(attributes, &stackaddr, &stacksize);
  if (!status)
    printf("Stack address = \x1b[1m%p\x1b[0m\nStack size = \x1b[1m%p bytes\x1b[0m\n\n", stackaddr, stacksize);

  status = pthread_attr_getscope(attributes, &scope);
  if (!status)
    printf("Scope: \x1b[1m%s\x1b[0m\n", (scope == PTHREAD_SCOPE_SYSTEM) ? "PTHREAD_SCOPE_SYSTEM" :
                                        (scope == PTHREAD_SCOPE_PROCESS) ? "PTHREAD_SCOPE_PROCESS" : "unknown");

  status = pthread_attr_getdetachstate(attributes, &detachstate);
  if (!status)
    printf("Detach state: \x1b[1m%s\x1b[0m\n", (detachstate == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
                                              (detachstate == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" : "unknown");

  status = pthread_attr_getguardsize(attributes, &guardsize);
  if (!status)
    printf("Guard size: \x1b[1m%p bytes\x1b[0m\n\n", guardsize);

  status = pthread_attr_getinheritsched(attributes, &inheritsched);
  if (!status)
    printf("Inherit scheduler: \x1b[1m%s\x1b[0m\n", (inheritsched == PTHREAD_INHERIT_SCHED) ? "PTHREAD_INHERIT_SCHED" :
                                                (inheritsched == PTHREAD_EXPLICIT_SCHED) ? "PTHREAD_EXPLICIT_SCHED" : "unknown");

  status = pthread_attr_getschedpolicy(attributes, &schedpolicy);
  if (!status)
    printf("Scheduling policy: \x1b[1m%s\x1b[0m\n", (schedpolicy == SCHED_OTHER) ? "SCHED_OTHER" :
                                                 (schedpolicy == SCHED_FIFO) ? "SCHED_FIFO" :
                                                 (schedpolicy == SCHED_RR) ? "SCHED_RR" : "unknown");

  status = pthread_attr_getschedparam(attributes, &schedule_parameters);
  if (!status)
    printf("Scheduling priority: \x1b[1m%d\x1b[0m\n", schedule_parameters.sched_priority);
}
