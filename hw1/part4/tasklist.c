#include <linux/module.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/string.h>

void printProcess(char *name, pid_t pid, int depth){
	char *spaces = kmalloc(4 * depth + 1, GFP_KERNEL);
	int i;
	for (i = 0; i < 4 * depth; i++){
		spaces[i] = ' ';
	}
	spaces[i] = '\0';
	printk(KERN_INFO "%s%s [%d]%s\n", spaces, depth != 0 ? "\\_" : "--", pid, name);
	kfree(spaces);
}
void dfs(struct task_struct *root, int depth){
	struct task_struct *child;
	printProcess(root->comm, root->pid, depth);
	list_for_each_entry(child, &root->children, sibling){
		dfs(child, depth+1);
	}
}

int tasklist_init(void)
{
	printk(KERN_INFO "Loading Module\n");
	dfs(&init_task, 0);
	return 0;
}
void tasklist_exit(void)
{
	printk(KERN_INFO "Removing Module\n");
}

module_init( tasklist_init );
module_exit( tasklist_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Tasklist Module");
MODULE_AUTHOR("SGG");
