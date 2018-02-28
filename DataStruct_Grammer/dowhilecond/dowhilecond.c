#include <stdio.h>

char Chains_getChar()
{
    char buffer[10];

    fflush(stdin);
    fgets(buffer, 10, stdin);

    return(buffer[0]);
}

int main(void)
{
    char ChannelsMenuChannelType[] =
    {
        "\r\n"
	"\r\n 0: Stream"
        "\r\n 1: H323"
        "\r\n 2: PCIe"
        "\r\n 3: Camera"
        "\r\n 4: Swms"
        "\r\n Enter Choice: "
    };

    char str[10];

    while(1)
    {
	printf("%s", ChannelsMenuChannelType);
	fgets(str, 10, stdin);
	if ((str[0] >= '0') && (str[0] <= '4') && (str[1] == '\n'))
	    break;
    }

    printf("%c\n", str[0]);
}


