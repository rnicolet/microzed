/*--------------------------------------------------------------------------------------\
    Interactive test that demonstrate how to use the command line parsing options class
    Use the class CGetOptions located in firmware/src/common/cpp folder

    Here are infromation needed to get the command line parsing into your private setting
    structure
    - short option table
    - long option table
    - Storage place for your private parameters
    - Handler how to process parameters.
\--------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "get-options.h"

struct USER_SETTINGS {
    int 	iMaxCnt;
    int 	iStep;
    char*	szOutput;
};

void usage()
{
    printf("Usage:\n");
    printf("\t-h --help. Show this help\n");
    printf("\t-m --max-cnt. Max value\n");
    printf("\t-s --step. Step value\n");
    printf("\t-o --output. Output file\n");
    printf("Example\n");
    printf("\t./parse-cmdline -m 100 -s 5 -o /tmp/parse-cmdline.txt\n");
}

/*--------------------------------------------------------------------------------------\
    Set of handler function callded from the switch statement in ProcessOption handler
    - OnUsage
    - OnMaxValue
    - OnStepValue
    - OnSetOutput
\--------------------------------------------------------------------------------------*/
bool OnUsage(int ch, char *optarg, struct USER_SETTINGS* pUsrSet)
{
    usage();
    return true;
}

bool OnMaxValue(int ch, char *optarg, struct USER_SETTINGS* pUsrSet)
{
    pUsrSet->iMaxCnt = strtol(optarg, NULL, 0);
    return false;
}

bool OnStepValue(int ch, char *optarg, struct USER_SETTINGS* pUsrSet)
{
    pUsrSet->iStep = strtol(optarg, NULL, 0);
    return false;
}

bool OnSetOutput(int ch, char *optarg, struct USER_SETTINGS* pUsrSet)
{
    pUsrSet->szOutput = optarg;
    return false;
}

/*--------------------------------------------------------------------------------------\
    Option handler provided when calling ParseCmdLine member function of CGetOptions
\--------------------------------------------------------------------------------------*/
bool ProcessOption(int ch, char *optarg, void* user_ctx)
{
    bool bDone = false;
    do {
        struct USER_SETTINGS* pUsrSet = (struct USER_SETTINGS*)user_ctx;
        if(pUsrSet == NULL) {
            break;
        }
        #define ON(msg, fn) case msg: bDone = fn(ch, optarg, pUsrSet); break;
        switch(ch) {
            ON('h', OnUsage);
            ON('m', OnMaxValue);
            ON('s', OnStepValue);
            ON('o', OnSetOutput);
        default:
            printf("Try `--help' for more information.\n");
            bDone = true;
            break;
        }
        #undef ON

    } while(false);

    return bDone;
}

/*--------------------------------------------------------------------------------------\
    TraceAndSaveInFile
    Function that uses command line options
\--------------------------------------------------------------------------------------*/
bool TraceAndSaveInFile(USER_SETTINGS& UsrSet)
{
    bool bRet = true;
    do {
        // Here we use what command line options has been set to.
        FILE* fd = fopen(UsrSet.szOutput, "w+");
        if(fd == NULL) {
            bRet = false;
            break;
        }

        char str[256] = {0};
        for(int i=0; i<UsrSet.iMaxCnt; i+=UsrSet.iStep) {
            int iRet = sprintf (str, "Reached %d of %d\n", i, UsrSet.iMaxCnt);
            size_t iSize = fwrite(str, sizeof(char), strlen(str), fd);
            printf("%s", str);
            if(iSize != strlen(str)) {
                bRet = false;
                break;	// an error occur time to exit
            }
        }
        fclose(fd);
    } while(false);

    return bRet;
}

/*--------------------------------------------------------------------------------------\
    main
\--------------------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	printf("%s. Built on %s at %s\n", argv[0], __DATE__, __TIME__);

    do {
        // Section that your application must define to use CGetOptions class
        const char *short_options    = "hm:s:o:";
        struct option long_options[] = {
            {"help"         , 0 , 0 , 'h'} ,
            {"max-cnt"      , 1 , 0 , 'm'} ,
            {"step-size"    , 1 , 0 , 's'} ,
            {"output"       , 1 , 0 , 'o'} ,
            {0              , 0 , 0 , 0}       // mark end of options
        };

        USER_SETTINGS settings = {
            100,
            5,
            (char*)"/dev/null"  // FileStream need it but we don't use it in this app
        };

        // Create an object of CGetOption. On the constructor pass
        // short and long options table as well as ptr to our personal
        // setting structure where to save our parameters.
        CGetOptions opt(short_options, long_options, &settings);
        bool bRet = opt.ParseCmdLine(argc, argv, ProcessOption);
        if(bRet) {
            break;  // command line not fully understood or help requested
        }

        // We are done with parsing the command line options
        printf("Max value: %d, step value: %d, output file: %s\n",
            settings.iMaxCnt, settings.iStep, settings.szOutput);

        TraceAndSaveInFile(settings);

    } while(false);

    return 0;
}
