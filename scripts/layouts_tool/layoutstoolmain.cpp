#include "layoutstoolmain.h"
#include <QDebug>
#include <QString>
#include <QProcess>

LayoutsToolMain::LayoutsToolMain(QObject *parent) :
        QObject(parent),
        mFilePath(LAYOUTS_CPP_PATH) {

    app = QCoreApplication::instance();
    pLayoutsFileHandler = new LayoutsFileHandler();

    QProcess::execute("export TERM=${TERM:-dumb}");
    system("export TERM=${TERM:-dumb}");
}

void LayoutsToolMain::run() {
    qDebug() << "Welcome to the Layouts tool :)";

    pLayoutsFileHandler->open(mFilePath, mLayouts);
    mainMenu();
}

void LayoutsToolMain::quit() {
    emit finished();
}

void LayoutsToolMain::aboutToQuitApp() {
    delete pLayoutsFileHandler;
}

void LayoutsToolMain::mainMenu() {
    QTextStream qtin(stdin);

    bool userWantsToQuit = false;
    do {
        bool loaded = !mFilePath.isEmpty();
        int menuChoice = 0;

        // Print menu
        clearScreen();
        qDebug() << "********** LAYOUT TOOLS MAIN MENU **********";
        if (loaded) qDebug() << "Currently opened file: " << mFilePath;
        qDebug() << "(1): Open file";
        qDebug() << "(2): Save file";
        if (loaded) qDebug() << "(3): Edit file";
        qDebug() << (loaded ? "(4): Quit" : "(3): Quit");

        // Prompt user for choice
        qtin >> menuChoice;

        switch(menuChoice) {
            case 1: {
                clearScreen();
                qDebug() << "Please tell me the path to the layouts cpp file (no spaces please): ";
                qtin >> mFilePath;
                pLayoutsFileHandler->open(mFilePath, mLayouts);
                break;
            }

            case 2: {
                qDebug() << "Save file...";
                break;
            }

            case 3: {
                if (loaded) {
                    editLayoutMenu();
                } else {
                    qDebug() << "Exit...";
                    quit();
                }
                break;
            }

            case 4: {
                if (loaded) {
                    userWantsToQuit = true;
                    quit();
                }
                break;
            }

            default:
                qDebug() << "ERROR! You have selected an invalid choice.";
                break;
        }
    } while (!userWantsToQuit);
}

void LayoutsToolMain::editLayoutMenu() {
    QTextStream qtin(stdin);
    bool loaded = !mFilePath.isEmpty();
    if (!loaded) {
        qDebug() << "Can't edit any layout, any layout loaded.";
        return;
    }


    bool backToMain = false;
    do {
        int menuChoice = 0;

        // Print menu
        clearScreen();
        qDebug() << "********** LAYOUT TOOLS - EDIT LAYOUT FILE **********";
        qDebug() << "Editing file: " << mFilePath;
        qDebug() << "(1): Remove layouts";
        qDebug() << "(2): Add layouts";
        qDebug() << "(3): Back to main menu";

        // Prompt user for choice
        qtin >> menuChoice;

        switch(menuChoice) {
            case 1: {
                // Remove layouts
                qDebug() << "Remove layouts...";
                break;
            }

            case 2: {
                // Add layouts
                qDebug() << "Add layouts...";
                break;
            }

            case 3: {
                // Back to main menu
                backToMain = true;
                break;
            }

            default:
                qDebug() << "ERROR! You have selected an invalid choice.";
                break;
        }
    } while (!backToMain);
}

void LayoutsToolMain::clearScreen() {
    bool termEnvVarDefined = QProcessEnvironment::systemEnvironment().contains("TERM");

    if (!termEnvVarDefined) {

        // TODO(Tomasito) Find a way of clearing the screen when TERM environment variable is not defined
        qDebug() << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

        return;
    }

    QProcess::execute("clear");
}