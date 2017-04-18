#include <linux/notifier.h>
#include <linux/export.h>

static BLOCKING_NOTIFIER_HEAD(hello_notifier_list);

/**
 *	hello_register_client - register a client notifier
 *	@nb: notifier block to callback on events
 */
int hello_register_client(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&hello_notifier_list, nb);
}
EXPORT_SYMBOL(hello_register_client);

/**
 *	hello_unregister_client - unregister a client notifier
 *	@nb: notifier block to callback on events
 */
int hello_unregister_client(struct notifier_block *nb)
{
	return blocking_notifier_chain_unregister(&hello_notifier_list, nb);
}
EXPORT_SYMBOL(hello_unregister_client);

/**
 * hello_notifier_call_chain - notify clients of hello_events
 *
 */
int hello_notifier_call_chain(unsigned long val, void *v)
{
	return blocking_notifier_call_chain(&hello_notifier_list, val, v);
}
EXPORT_SYMBOL_GPL(hello_notifier_call_chain);

