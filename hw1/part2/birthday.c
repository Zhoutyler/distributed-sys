#include <linux/module.h>
#include <linux/list.h>
#include <linux/printk.h>
#include <linux/slab.h>

#define	N		5
#define MAGIC_DAY	1
#define MAGIC_MONTH	4
#define MAGIC_YEAR	1969

#define MK_DAY(d)	(MAGIC_DAY + d)
#define MK_MONTH(m)	(MAGIC_MONTH + m)
#define MK_YEAR(y)	(MAGIC_YEAR + y)

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

/* TODO: declare a single static struct list_head, named birthday_list */
static LIST_HEAD(birthday_list);

void add_birthday(int day, int month, int year)
{
	struct birthday *birth_day;
	birth_day = kmalloc(sizeof(*birth_day), GFP_KERNEL);
	birth_day -> day = day;
	birth_day -> month = month;
	birth_day -> year = year;
	INIT_LIST_HEAD(&birth_day -> list);
	list_add_tail(&birth_day -> list, &birthday_list);
}

void print_birthdays(void)
{
	struct birthday *p;
	list_for_each_entry(p, &birthday_list, list){
		printk(KERN_INFO "day: %d, month: %d, year: %d\n", p -> day, p -> month, p -> year);
	}

}

void delete_birthdays(void)
{
	struct birthday *p, *next;
	list_for_each_entry_safe(p, next, &birthday_list, list){
		kfree(p);
	}
}

int birthday_init(void)
{
	unsigned int i;

	printk(KERN_INFO "Loading Module\n");

	for (i = 0; i < N; i++)
		add_birthday(MK_DAY(i), MK_MONTH(i), MK_YEAR(i));

	print_birthdays();
	return 0;
}

void birthday_exit(void)
{
	printk(KERN_INFO "Removing Module\n");

	print_birthdays();

	delete_birthdays();
}

module_init( birthday_init );
module_exit( birthday_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A handy birthday list module");
MODULE_AUTHOR("SGG");
