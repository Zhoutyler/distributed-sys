#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>


/*
 * Undefine commonly used macros -- DO NOT MODIFY
 * Please also do not use other macros and functions defined in <linux/list.h>,
 * such as INIT_LIST_HEAD, list_add, list_add_tail, list_del, list_empty,
 * or any of the internal helper functions beginning with __.
 */
#undef LIST_HEAD
#undef LIST_HEAD_INIT
#undef list_entry
#undef list_for_each_entry
#undef list_for_each_entry_safe
#undef container_of
#undef offsetof

#define	N		5
#define MAGIC_DAY	1
#define MAGIC_MONTH	4
#define MAGIC_YEAR	1969

#define MK_DAY(d)	(MAGIC_DAY + d)
#define MK_MONTH(m)	(MAGIC_MONTH + m)
#define MK_YEAR(y)	(MAGIC_YEAR + y)
#define offsetof(type, member) ((size_t)&( ((struct type *)0) -> member ))
#define container_of(ptr, type, member) ({      \
		const typeof( ((struct type*)0)->member) *__mptr = (ptr); \
		(struct type *)( (char *)__mptr - offsetof(type, member) );})


struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};

void print_birthday(struct birthday *b)
{
	printk(KERN_INFO "Birthday: %d-%d-%d\n", b->year, b->month, b->day);
}

static struct list_head birthday_list = {&birthday_list, &birthday_list};

void add_birthday(int day, int month, int year)
{
	struct birthday *birth_day;
	birth_day = kmalloc(sizeof(*birth_day), GFP_KERNEL);
	birth_day -> day = day;
	birth_day -> month = month;
	birth_day -> year = year;
	birth_day -> list.prev = &birth_day -> list;
	birth_day -> list.next = &birth_day -> list;
	
	birth_day -> list.next = &birthday_list;
	birth_day -> list.prev = birthday_list.prev;
	birthday_list.prev -> next = &birth_day -> list;
	birthday_list.prev = &birth_day -> list;
}

void print_birthdays(void)
{
	struct birthday *birthday_ptr;
	struct list_head *p;
	for (p = birthday_list.next;
			p != &birthday_list;
			p = p -> next) {
		birthday_ptr = container_of(p, birthday, list);
		print_birthday(birthday_ptr);
	}
}

void delete_birthdays(void)
{	
	struct birthday *birthday_ptr;
	struct list_head *p, *n;
	for (p = birthday_list.next, n = p -> next;
			p != &birthday_list;
			p = n, n = n -> next){
		birthday_ptr = container_of(p, birthday, list);
		kfree(birthday_ptr);
	}
}

int birthday_nom_init(void)
{
	unsigned int i;

	printk(KERN_INFO "Loading Module\n");

	for (i = 0; i < N; i++)
		add_birthday(MK_DAY(i), MK_MONTH(i), MK_YEAR(i));

	print_birthdays();

	return 0;
}

void birthday_nom_exit(void)
{
	printk(KERN_INFO "Removing Module\n");

	print_birthdays();

	delete_birthdays();
}

module_init( birthday_nom_init );
module_exit( birthday_nom_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A handy birthday list module, without list macros");
MODULE_AUTHOR("SGG");
