#ifndef HELLOWORLD_NOTIFY_H
#define HELLOWORLD_NOTIFY_H

extern int hello_register_client(struct notifier_block *nb);
extern int hello_unregister_client(struct notifier_block *nb);
extern int hello_notifier_call_chain(unsigned long val, void *v);

#endif
