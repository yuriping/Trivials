/*
 * 8.2
 * 查看进程的各个 id，进程 id，父进程，实际用户、组，有效用户、组
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{
    pid_t pid, ppid;
    uid_t uid, euid;
    gid_t gid, egid;

    /* 进程与父进程 Id */
    pid = getpid();
    ppid= getppid();
    printf("<pid> %d, <ppid> %d\n", pid, ppid);

    /* 实际、有效用户 Id */
    // 有效用户 Id ，一般就是实际用户 Id，但在设置用户 Id 时，是所有者用户 Id
    uid = getuid();
    euid = geteuid();
    printf("<uid> %d, <euid> %d\n", uid, euid);

    /* 实际、有效组 Id */
    // 有效组 Id ，一般就是实际组 Id，但在设置组 Id 时，是所有者组 Id
    gid = getgid();
    egid = getegid();
    printf("<gid> %d, <egid> %d\n", gid, egid);

    /* 所有者 Id */
    /*
     * TODO : 可通过 lstat 函数得到 struct stat 中的 st_udi、st_gid 得到
     */

    exit(0);
}
