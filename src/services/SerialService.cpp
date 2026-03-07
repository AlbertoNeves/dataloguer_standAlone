#include <Arduino.h>
#include <SdFat.h>

#include "SerialService.h"
#include "SDService.h"
#include "RTCService.h"

#include "LogEngine.h"
#include "../core/SystemState.h"

static String command;

static DateTime tempDate;
static char dumpSensor;

//------------------------------------------------
// Estados do CLI
//------------------------------------------------
enum CliState
{
    CLI_IDLE,
    CLI_DUMP_SENSOR,
    CLI_DUMP_DATE,
    CLI_SET_DATE,
    CLI_SET_TIME
};

static CliState cliState = CLI_IDLE;

//------------------------------------------------
static void status()
{
    Serial.println("========== STATUS ==========");

    DateTime now = RTC_GetDateTime();

    Serial.print("RTC: ");
    Serial.print(now.day());
    Serial.print("/");
    Serial.print(now.month());
    Serial.print("/");
    Serial.print(now.year());
    Serial.print(" ");

    Serial.print(now.hour());
    Serial.print(":");
    Serial.print(now.minute());
    Serial.print(":");
    Serial.println(now.second());

    Serial.print("Logging: ");
    Serial.println(Log_IsActive() ? "ATIVO" : "PARADO");

    Serial.println("============================");
}

//------------------------------------------------
static void sdcheck()
{
    Serial.println("Verificando SD...");

    if(SD_Check())
        Serial.println("SD OK");
    else
        Serial.println("SD ERROR");
}

//------------------------------------------------
static void logstart()
{
    Log_Start();
    Serial.println("Logging iniciado");
}

//------------------------------------------------
static void logstop()
{
    Log_Stop();
    Serial.println("Logging parado");
}

//------------------------------------------------
static void dir()
{
    FsFile dir;
    FsFile file;

    if(!dir.open("/"))
    {
        Serial.println("Erro abrindo diretorio");
        return;
    }

    Serial.println("Arquivos no SD:");

    while(file.openNext(&dir, O_RDONLY))
    {
        char name[32];
        file.getName(name,sizeof(name));

        Serial.println(name);

        file.close();
    }

    dir.close();

    Serial.println("----------------------");
}

//------------------------------------------------
static void date()
{
    DateTime now = RTC_GetDateTime();

    Serial.print("Data: ");

    if(now.day()<10) Serial.print("0");
    Serial.print(now.day());
    Serial.print("/");

    if(now.month()<10) Serial.print("0");
    Serial.print(now.month());
    Serial.print("/");

    Serial.print(now.year());

    Serial.print("  Hora: ");

    if(now.hour()<10) Serial.print("0");
    Serial.print(now.hour());
    Serial.print(":");

    if(now.minute()<10) Serial.print("0");
    Serial.println(now.minute());
}

//------------------------------------------------
//------------------------------------------------
static void setdate()
{
    tempDate = RTC_GetDateTime();

    Serial.println("Data atual:");
    date();

    Serial.println("Digite nova data (DD/MM/YYYY)");
    Serial.println("ENTER para manter atual");

    cliState = CLI_SET_DATE;
}
//------------------------------------------------
static void listarDatas(char sensor)
{
    FsFile dir;
    FsFile file;

    if(!dir.open("/"))
    {
        Serial.println("Erro abrindo diretorio raiz");
        return;
    }

    Serial.println("Datas disponiveis:");

    while(file.openNext(&dir,O_RDONLY))
    {
        char name[20];
        file.getName(name,sizeof(name));

        if(strlen(name) >= 8 && name[7]==sensor)
        {
            name[6] = 0;   // remove "_X.CSV"
            Serial.println(name);
        }

        file.close();
    }

    dir.close();
}

//------------------------------------------------
static void dumpArquivo(const char *filename)
{
    FsFile file = SD.open(filename, O_READ);

    if(!file)
    {
        Serial.println("Arquivo nao encontrado");
        return;
    }

    Serial.print("Dados presentes no arquivo ");
    Serial.println(filename);

    Serial.println("================================");

    while(file.available())
        Serial.write(file.read());

    Serial.println("================================");

    file.close();
}

//------------------------------------------------
static void help()
{
    Serial.println("=========== COMANDOS ==========");

    Serial.println("help      - ajuda");
    Serial.println("status    - status sistema");

    Serial.println("dir       - lista arquivos SD");
    Serial.println("sdcheck   - verifica SD");

    Serial.println("date      - mostra data/hora");
    Serial.println("setdate   - ajustar RTC");

    Serial.println("logstart  - iniciar log");
    Serial.println("logstop   - parar log");

    Serial.println("dump      - extrair dados");

    Serial.println("================================");
}

//------------------------------------------------
void SerialService_Init()
{
}

//------------------------------------------------
void SerialService_Update()
{
    if(!Serial.available())
        return;

    command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();

    //------------------------------------------------
    // AJUSTE DATA
    //------------------------------------------------
    if(cliState == CLI_SET_DATE)
    {
        if(command.length() > 0)
        {
            int d,m,y;
            sscanf(command.c_str(),"%d/%d/%d",&d,&m,&y);

            tempDate = DateTime(y,m,d,
                                tempDate.hour(),
                                tempDate.minute(),
                                0);
        }

        Serial.println("Digite hora (HH:MM)");
        Serial.println("ENTER para manter atual");

        cliState = CLI_SET_TIME;
        return;
    }

    //------------------------------------------------
    // AJUSTE HORA
    //------------------------------------------------
if(cliState == CLI_SET_TIME)
{
    if(command.length() > 0)
    {
        int h,m;
        sscanf(command.c_str(),"%d:%d",&h,&m);

        tempDate = DateTime(tempDate.year(),
                            tempDate.month(),
                            tempDate.day(),
                            h,m,0);
    }

    RTC_SetDateTime(tempDate);

    Serial.println("RTC atualizado");

    date();   // mostra nova data/hora

    cliState = CLI_IDLE;
    return;
}
    //------------------------------------------------
    // ESTADO NORMAL
    //------------------------------------------------
    if(cliState == CLI_IDLE)
    {
        if(command == "help")
            help();

        else if(command == "status")
            status();

        else if(command == "dir")
            dir();

        else if(command == "date")
            date();

        else if(command == "setdate")
            setdate();

        else if(command == "sdcheck")
            sdcheck();

        else if(command == "logstart")
            logstart();

        else if(command == "logstop")
            logstop();

        else if(command == "dump")
        {
            Serial.println("Qual sensor?");
            Serial.println("T - Temperatura");
            Serial.println("H - Umidade");
            Serial.println("P - Pressao");

            cliState = CLI_DUMP_SENSOR;
        }
        else
        {
            Serial.println("Comando invalido (digite help)");
        }

        return;
    }

    //------------------------------------------------
    // ESCOLHA SENSOR
    //------------------------------------------------
    if(cliState == CLI_DUMP_SENSOR)
    {
        dumpSensor = toupper(command[0]);

        if(dumpSensor!='T' && dumpSensor!='H' && dumpSensor!='P')
        {
            Serial.println("Sensor invalido");
            cliState = CLI_IDLE;
            return;
        }

        listarDatas(dumpSensor);

        Serial.println("Digite data (YYMMDD)");

        cliState = CLI_DUMP_DATE;
        return;
    }

    //------------------------------------------------
    // ESCOLHA DATA
    //------------------------------------------------
    if(cliState == CLI_DUMP_DATE)
    {
        char filename[16];

        sprintf(filename,"%s_%c.CSV",
                command.c_str(),
                dumpSensor);

        dumpArquivo(filename);

        cliState = CLI_IDLE;
        return;
    }
}