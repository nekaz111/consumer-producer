//JEREMIAHTHSIEHTICSIT500TPROJECTT2TPRODUCER
//ACCEPTSTMULTIPLETCONSUMERTREQUESTSTANDTPROCESSESTEACHTREQUESTTBYTCREATINGTFOURTTHREADS
//1.TTHETREADERTTHREADTWILLTREADTANTINPUTTFILE,TONETLINETATTATTIME.TITTWILLTPASSTEACHTLINETOFTINPUTTTOTTHETCHARACTERTTHREADTTHROUGHTATQUEUETOFTMESSAGES.
//2.TTHETCHARACTERTTHREADTCOMPONENTTWILLTSCANTTHETLINETANDTREPLACETEACHTBLANKTCHARACTERTBYTTHETCHARACTERTSUPPLIEDTBYTTHETCLIENT.TITTWILLTTHENTPASSTTHETLINETTOTTHETTOUPPERTTHREADTTHROUGHTANOTHERTQUEUETOFTMESSAGES.
//3.TTHETTOUPPERTTHREADTCOMPONENTTWILLTSCANTTHETLINETANDTCONVERTTALLTLOWERTCASETLETTERSTTOTUPPERTCASE.TITTWILLTTHENTPASSTTHETCONVERTEDTLINETTOTTHETWRITERTTHREADTTHROUGHTANOTHERTQUEUETOFTMESSAGES.
//4.TTHETWRITERTTHREADTWILLTWRITETTHETLINETTOTANTOUTPUTTFILE.
//PRODUCERTISTSERVERTANDTCONSUMERTISTCLIENT?

#INCLUDET<STRING.H>
#INCLUDET<FSTREAM>
#INCLUDET<IOSTREAM>
#INCLUDET<PTHREAD.H>
#INCLUDET<UNISTD.H>
#INCLUDET<STDLIB.H>
#INCLUDET<STDIO.H>
#INCLUDET<QUEUE>


#INCLUDET<ARPA/INET.H>
#INCLUDET<SYS/TYPES.H>
#INCLUDET<SYS/SOCKET.H>
#INCLUDET<NETINET/IN.H>
#INCLUDET<CTYPE.H>
#INCLUDET<VECTOR>
#INCLUDET<STRING>


//PROTOTYPES
INTTCHARACTER(STD::STRINGTLINE,TCHARTC);
INTTTOUPPER(STD::STRINGTLINE);
INTTWRITER(STD::STRINGTLINE);


//USETGLOBALTVARIABLESTFORTSHAREDTDATA?
STD::STRINGTREADFILET=T"FILE.TXT";
STD::QUEUET<STD::STRING>TFILELINES,TCHARLINES,TUPPERLINES;
CHARTCT=T'H';
INTTFILEENDT=T0,TCHARENDT=T0,TUPPERENDT=T0;
STD::STRINGTFILETEXT;
//QUEUETLOCKTFLAG
//NOTTNEEDEDTSINCETWETARETNOTTACCESSINGTTHETSAMETQUEUETWITHTDIFFERENTTTHREADS?T(INSTEADTWETARETUSINGTSEPERATETQUEUES?)
PTHREAD_MUTEX_TTMUTEXT=TPTHREAD_MUTEX_INITIALIZER;T
PTHREAD_COND_TTTOUPPERSTATT=TPTHREAD_COND_INITIALIZER;T
PTHREAD_COND_TTCHARACTERSTATT=TPTHREAD_COND_INITIALIZER;

//INTTHISTCASE,TSHAREDTDATATTHATTMUSTTBETPROTECTEDTBYTSEMAPHORETTIMINGTISTTHETLINETBEINGTPROCESSEDT(?)

//TAKESTFILENAMETANDTPUSHESTLINESTFROMTFILETTOTQUEUETFORTCHARACTERTTOTREAD
VOIDT*READER(VOIDT*FILENAME)
{
T//LOCKTMUTEX
T//PTHREAD_MUTEX_LOCK(&MUTEX);
TTTTTTTTSTD::IFSTREAMTINFILE;
TTTTTTTTSTD::STRINGTLINE;
T//TYPECASTTFILENAMETFROMTVOIDTTOTSTRING
TTTTTTTT//INFILE.OPEN(*(STD::STRING*)FILENAME);
TTTTTTTTINFILE.OPEN(READFILE);
TIFT(INFILE.IS_OPEN())
TTTTTTTT{
TTTTTTTTTTTTTTTTWHILE(GETLINE(INFILE,TLINE))
TTTTTTTTTTTTTTTT{
TTT//CHARACTER(LINE,T'C');
TTTTTTTTTTTTTTTTTTTTTTTT//ADDTLINETTOTQUEUE
TTTFILELINES.PUSH(LINE);
TTT//

TTTTTTTTTTTTTTTT}
TTTTTTTT}
TTTTTTTTELSE
TTTTTTTT{
TTTTTTTTTTTTTTTTPRINTF("FILETNOTTOPENED");
TTTTTTTT}
TFILEENDT=T1;
TRETURNTNULL;
}


//TAKESTLINESTFROMTREADERTANDTREPLACESTEACHTBLANKTCHARACTERT(SPACE)TWITHTSPECIFIEDTCHAR
//PUSHESTRESULTTTOTQUEUETFORTTOUPPERTTOTREAD
VOIDT*CHARACTER(VOIDT*VAR)
{
T//RUNTTHREADTUNTILTQUITTCONDITION
TWHILE(1)
T{
TT//LOCKTMUTEX
TT//PTHREAD_MUTEX_LOCK(&MUTEX);
TT//CHECKTIFTQUEUETHASTELEMENTS
TTIFT(FILELINES.SIZE()T>T0)
TT{
TTT//GETTLINETFROMTQUEUE
TTTSTD::STRINGTLINET=TFILELINES.FRONT();
TTTTTTTTTT//PRINTF("%S\N",TLINE.C_STR());
TTT//LOOPTTHROUGHTLINE
TTTTTTTTTTFORT(INTTXT=T0;TXT<TLINE.LENGTH();TX++)
TTTTTTTTTT{
TTTTTTTTTTTTTTTTTT//CHECKTIFTSPACE
TTTTTTTTTTTTTTTTTTTIFT(ISBLANK(LINE[X]))
TTTTTTTTTTTTTTTTTT{
TTTTTTTTTTTTTTTTTTTTTTTTTT//PRINTF("%C",TLINE[X]);
TTTTTTTTTTTTTTTTTTTTTTTTTTT//REPLACETWITHTCHARACTER
TTTTTTTTTTTTTTTTTTTTTTTTTTTLINE[X]T=TC;
TTTTTTTTTTTTTTTTTT}
TTTTTTTTTT}
TTTTTTTTTT//STD::COUTT<<TLINET<<TSTD::END;
TTTTTTTTTTFILELINES.POP();
TTTCHARLINES.PUSH(LINE);
TTT//TOUPPER(LINE);
TTT//PTHREAD_COND_SIGNAL(&TOUPPERSTAT);
TT}
TT//ENDTLOOPTCONDITION
TTELSETIFT(FILEENDT==T1T&&TFILELINES.SIZE()T==T0)
TT{
TTTCHARENDT=T1;
TTTRETURNTNULL;
TT}
TT//WAITTIFTOTHERTTHREADTISTEXECUTINGT(DON'TTNEEDTTHIS?)
TTELSE
TT{
TTT//PTHREAD_COND_WAIT(&CHARACTERSTAT,T&MUTEX);T
TT}
TT//UNLOCKTMUTEX
TT//PTHREAD_MUTEX_UNLOCK(&MUTEX);
T}
}



//GETSTLINETFROMTCHARACTERTQUEUETANDTCONVERTTTOTUPPERCASE
//PUSHESTRESULTTTOTWRITERTQUEUE
VOIDT*TOUPPER(VOIDT*VAR)
{
T//RUNTTHREADTUNTILTQUITTCONDITION
TWHILE(1)
T{
TT//MUTEXTLOCK
TT//PTHREAD_MUTEX_LOCK(&MUTEX);
TTIFT(CHARLINES.SIZE()T>T0)
TT{
TTT//GETTLINETFROMTQUEUE
TTTSTD::STRINGTLINET=TCHARLINES.FRONT();
TTT//LOOPTTHROUGHTLINE
TTTTTTTTTTFORT(INTTXT=T0;TXT<TLINE.LENGTH();TX++)
TTTTTTTTTT{
TTTTTTTTTTTTTTTTTT//CONVERTTTOTUPPERCASE
TTTTTTTTTTTTTTTTTTLINE[X]T=TTOUPPER(LINE[X]);
TTTTTTTTTT}
TTTTTTTTTT//STD::COUTT<<TLINET<<TSTD::ENDL;
TTT
TTT//REMOVETFIRSTTLINETSINCETITTHASTNOWTBEENTCOMPLETELYTUSEDTUP
TTTCHARLINES.POP();
TTT//ADDTLINETTOTWRITERTQUEUE
TTTUPPERLINES.PUSH(LINE);
TTT//PTHREAD_COND_SIGNAL(&CHARACTERSTAT);
TT}
TT//ENDTCONDITION
TTELSETIFT(CHARENDT==T1T&&TCHARLINES.SIZE()T==T0)
TT{
TTTUPPERENDT=T1;
TTTRETURNTNULL;
TT}
TTELSE
TTTTTTTTTTTTTTTT{
TTTTTTTTTTTTTTTTTTTTTTTT//PTHREAD_COND_WAIT(&TOUPPERSTAT,T&MUTEX);
TTTTTTTTTTTTTTTT}
TT//UNLOCKTMUTEX
TTTTTTTTTTTTTTTT//PTHREAD_MUTEX_UNLOCK(&MUTEX);
T}
}


//WRITETLINETTOTOUTPUTTFILE
VOIDT*WRITER(VOIDT*VAR)
{
TTTTTTTTSTD::STRINGTFILENAMET=TREADFILE;
TTTTTTTTSTD::OFSTREAMTOUTFILE;
TTTTTTTT//APPENDTTOTFILENAME
TTTTTTTTFILENAME.INSERT(0,T"CONVERTED");
TTTTTTTT//OPENTFILE
TTTTTTTTOUTFILE.OPEN(FILENAME);
TTTTTTTT//IFTFILETISTOPEN
TIFT(OUTFILE.IS_OPEN())
TTTTTTTT{
TTSTD::STRINGTLINE;
TTTTTTTTTTTTTTTTWHILE(1)
TT{
TTT//CHECKTQUEUE
TTTIFT(UPPERLINES.SIZE()T>T0)
TTT{
TTTT//GETTLINETFROMTQUEUE
TTTTLINET=TUPPERLINES.FRONT();
TTTT//WRITETTOTFILE
TTTTOUTFILET<<TLINET<<TSTD::ENDL;
TTTTFILETEXTT+=TLINET+T"\N";
TTTT//REMOVETLINETFROMTQUEUE
TTTTUPPERLINES.POP();T
TTT}
TTT//ENDTCONDITION
TTTELSETIFT(UPPERENDT==T1T&&TUPPERLINES.SIZE()T==T0)
TTT{
TTTTOUTFILE.CLOSE();
TTTT//RETURNINGTFILENAMETNOTTWORKING
TTTT//RETURNT(VOIDT*)FILENAME;
TTTTRETURNTNULL;
TTT}
TT}
TTTTTTTT}
T//IFTFILETISTNOTTOPEN
TTTTTTTTELSE
TTTTTTTT{
TTTTTTTTTTTTTTTTPRINTF("FILETNOTTOPENED");
TTTTTTTT}
TOUTFILE.CLOSE();
TRETURNTNULL;
}


VOIDTERROR(CONSTTCHART*MSG)
{
TTTTPERROR(MSG);
TTTTEXIT(1);
}



//MAINTFUNCTION
INTTMAIN(INTTARGC,TCHART*ARGV[])
{
T//GETTINPUTTFROMTCLIENTTCONSUMER
T//CODETISTFROMTLABT9TSERVER/CLIENTTIMPLEMENTATIONTEXCEPTTTHATTFORTTESTINGTEFFICIENCYTITTHASTBEENTHARDCODEDTFORTLOCALMACHINETONLY
T
TINTTSOCKFD,TNEWSOCKFD,TPORTNO;
TSOCKLEN_TTCLILEN;
TTTTTCHARTBUFFER[256];
TTTTTTSTRUCTTSOCKADDR_INTSERV_ADDR,TCLI_ADDR;
TTTTTTINTTN;

TTTTTTIFT(ARGCT<T2)T
T{
TTTTTTTTTFPRINTF(STDERR,"ERROR,TNOTPORTTPROVIDED\N");
TTTTTTTTTEXIT(1);
TTTTTT}
TTTTTTFPRINTF(STDOUT,T"RUNTCLIENTTBYTPROVIDINGTHOSTTANDTPORT\N");
TTTTTTSOCKFDT=TSOCKET(AF_INET,TSOCK_STREAM,T0);
TTTTTTIFT(SOCKFDT<T0)
TTTTTTTTTERROR("ERRORTOPENINGTSOCKET");

TTTTTTBZERO((CHART*)T&SERV_ADDR,TSIZEOF(SERV_ADDR));
TTTTTTPORTNOT=TATOI(ARGV[1]);
TTTTTSERV_ADDR.SIN_FAMILYT=TAF_INET;
TTTTTSERV_ADDR.SIN_ADDR.S_ADDRT=TINADDR_ANY;
TTTTTSERV_ADDR.SIN_PORTT=THTONS(PORTNO);
TTTTTIFT(BIND(SOCKFD,T(STRUCTTSOCKADDRT*)T&SERV_ADDR,
TTTTTTTTTTTTTTSIZEOF(SERV_ADDR))T<T0)
TTTTTTTTTTTTTTERROR("ERRORTONTBINDING");
TTTTTLISTEN(SOCKFD,5);

TTTTTCLILENT=TSIZEOF(CLI_ADDR);
TTTTT//WHILETLOOPTTOTKEEPTLISTENINGTFORTCLIENTS
TTTTTWHILE(TRUE)
TTTTT{
TTTTTTPRINTF("WAITINGTFORTCLIENT...\N");
TTTTTTNEWSOCKFDT=TACCEPT(SOCKFD,
TTTTTTTTTTTTTT(STRUCTTSOCKADDRT*)T&CLI_ADDR,
TTTTTTTTTTTTTTTTTT&CLILEN);
TTTTTTIFT(NEWSOCKFDT<T0)
TTTTTTTTTERROR("ERRORTONTACCEPT");
TTTTTTBZERO(BUFFER,256);
TTTTTTNT=TREAD(NEWSOCKFD,BUFFER,255);
TTTTTTIFT(NT<T0)
TTTTTTTTTERROR("ERRORTREADINGTFROMTSOCKET");
TT//BUFFERTISTSTRINGTRECIEVEDTFROMTCLIENT
TT//PRINTF("HERETISTTHETCOMMAND:T%S\N",TBUFFER);
TT//EXITTCOMMANDTFROMTCLIENT
TTIFT(STRCMP(BUFFER,T"EXIT\N")T==T0)
TT{
TTTTTTTTTPRINTF("EXITTPROGRAM\N");
TTTTTTTTTCLOSE(NEWSOCKFD);
TTTTTTTTTCLOSE(SOCKFD);
TTTTTTTTTEXIT(1);
TT}
TT//PARSETCOMMANDSTFROMTBUFFERT
TT//VECTORTSTORINGTPARSEDTSTRINGS
TTSTD::VECTOR<CHART*>TPARSED;
TTCHAR*TSPLIT;
TTTTTTT//USETSTRTOKTTOTSPLITTBUFFERTUSINGTSPECIFIEDTTOKENS
TTTTTTTFOR(SPLITT=TSTRTOK(BUFFER,T"T<,>");TSPLITT!=TNULL;TSPLITT=TSTRTOK(NULL,"T<.>"))
TTTTTTT{
TTTTTTTTTT//STORETPARSEDTSTRINGS
TTTTTTTTTTPARSED.PUSH_BACK(SPLIT);
TTTTT}
T


TT//NOTERRORTCHECKINGTTAKETPARAMETERS
TTREADFILET=TPARSED[0];T
TTCT=TPARSED[1][0];

TTPTHREAD_TTTHREAD1,TTHREAD2,TTHREAD3,TTHREAD4;
TT//PRINTF("BEFORETTHREADS\N");
TTVOIDT*WRITEFILE;
TT//MAKETTHREADSTANDTEXECUTETFUNCTIONS
TTPTHREAD_CREATE(&THREAD1,TNULL,TREADER,TNULL);
TTPTHREAD_CREATE(&THREAD2,TNULL,TCHARACTER,TNULL);
TTPTHREAD_CREATE(&THREAD3,TNULL,TTOUPPER,TNULL);T
TTPTHREAD_CREATE(&THREAD4,TNULL,TWRITER,TNULL);
TT
TT//RUNTANDTSYNCTTHREADS
TTPTHREAD_JOIN(THREAD1,TNULL);
TTPTHREAD_JOIN(THREAD2,TNULL);
TTPTHREAD_JOIN(THREAD3,TNULL);
TT//CHART*FILETEXT;
TT//RETURNINGTSTRINGTNOTTWORKING
TT//PTHREAD_JOIN(THREAD4,T(VOID**)FILETEXT);
TTPTHREAD_JOIN(THREAD4,TNULL);
TT//PRINTF("%S\N",TFILETEXT.C_STR());
TT//PRINTF("AFTERTTHREADS\N");
TT//RETURNT1;
TT
TT//STD::STRINGTMESSAGET=T"CONVERTEDTFILETNAMETIS:T";T
TTTTTTTTT//MESSAGE.APPEND((STD::STRING)WRITEFILE);
TT//PRINTF("%S\N",TMESSAGE.C_STR());
TTPRINTF("FILETCONVERTED\N");
TT//NT=TWRITE(NEWSOCKFD,"ITGOTTYOURTCOMMMAND",19);
TT//SENDTCONVERTEDTTEXTTTOTCLIENT
TTNT=TWRITE(NEWSOCKFD,T"\NCONVERTEDTFILETCONTENTS:",T29);
TTNT=TWRITE(NEWSOCKFD,TFILETEXT.C_STR(),TFILETEXT.LENGTH());
T
TTIFT(NT<T0)
TTTTTTTTTTTTTTTTTERROR("ERRORTWRITINGTTOTSOCKET");
T
TTTTTTTTTCLOSE(NEWSOCKFD);
T}
TTTTTTTTCLOSE(SOCKFD);

}

