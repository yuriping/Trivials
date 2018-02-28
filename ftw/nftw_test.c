#define _XOPEN_SOURCE 500

#include<ftw.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>

#define FILEOPEN 1024

int gb_filecount;
int getMetadata(const char * dirpath, const struct stat * sb, int typeflag, struct FTW * ftwbuf);


int main(int argc, char ** argv)
{
    int ret = 0;
    struct stat pathbuf;
    if(argc > 2)
    {
        printf("-nfwt_t:invalid arguments \n ");
        return -1;
    }
    if (stat(argv[1], &pathbuf))
    {
        printf("-nfwt_t:invalid dirpath:%s\n", argv[1]);
        return -1;
    }
    else
    {
        if (0 == S_ISDIR(pathbuf.st_mode))
        {
            printf("-nfwt_t:/\"%s/\" is not dirpath\n", argv[1]);
            return -1;
        }
    }

    gb_filecount=0;
    ret = nftw(argv[1], getMetadata, FILEOPEN, FTW_PHYS);
    if (ret < 0)
    {
        printf("-nftw:[wrong:%d]ntfw search %d files\n", ret, gb_filecount);
        return -1;
    }
    else
    {
        printf("-nftw:[done:%d]trasvers in %s search %d files\n", ret, argv[1], gb_filecount);
        return 0;
    }
}

int getMetadata(const char * dirpath, const struct stat * sb, int typeflag, struct FTW * ftwbuf)
{
    printf("num:%d path:%s\n", ++gb_filecount, dirpath);
    printf("st_dev:%d\n", (*sb).st_dev);
    printf("st_ino:%d\n", (*sb).st_ino);
    printf("st_mode:%d S_ISDIR:%d\n", (*sb).st_mode, S_ISDIR((*sb).st_mode));
    printf("st_nlink:%d\n", (*sb).st_nlink);
    printf("st_uid:%d\n", (*sb).st_uid);
    printf("st_gid:%d\n", (*sb).st_gid);
    printf("st_rdev:%d\n", (*sb).st_rdev);
    printf("st_size:%d\n", (*sb).st_size);
    printf("st_blksize:%lu\n", (*sb).st_blksize);
    printf("st_blocks:%lu\n", (*sb).st_blocks);
    printf("st_atime:%ld\n", (*sb).st_atime);
    printf("st_mtime:%ld\n", (*sb).st_mtime);
    printf("st_ctime:%ld\n", (*sb).st_ctime);
    printf("typeflag:%d\n", typeflag);
    printf("FTW_base:%d FTW_level:%d \n", (*ftwbuf).base, (*ftwbuf).level);

    printf("================================================================\n");

    return 0;
}
