/* This file is part of Prometheus.

Prometheus is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
any later version.

Prometheus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Prometheus.  If not, see <http://www.gnu.org/licenses/>. */

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    proc(new QProcess(this))
{
    ui->setupUi(this);

    connect(proc, SIGNAL(started()),this, SLOT(procStarted()));
    connect(proc, SIGNAL(error(QProcess::ProcessError)),this, SLOT(procError(QProcess::ProcessError)));
    connect(proc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(procExited(int,QProcess::ExitStatus)));
    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(progStandardOutput()));
    connect(proc, SIGNAL(readyReadStandardError()), this, SLOT(progStandardError()));

    setDefaultConsoleColor();

    QSettings settings("Koc University", "Prometheus");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(500, 400)).toSize();
    resize(size);
    move(pos);
    fireprog_path = settings.value("fireprog_path", "").toString();
    bitfile_path = settings.value("bitfile_path", "").toString();
	#ifdef __MINGW32__
        ui->lineEdit_fireprog->setText("fireprog.exe"); //fireprog_file.setFile("fireprog");
    #elif defined(__MINGW64__)
        ui->lineEdit_fireprog->setText("fireprog.exe");//fireprog_file.setFile("fireprog.exe");
    #else
        ui->lineEdit_fireprog->setText("fireprog"); //fireprog_file.setFile("fireprog.exe");
    #endif


    if (!bitfile_path.isEmpty()){
        bit_file.setFile(bitfile_path);
        if (bit_file.exists() && bit_file.isFile()) {
            ui->lineEdit_bitfile->setText(bit_file.canonicalFilePath());
        }
    }
    if (!bscan_path.isEmpty()){
        bscan_file.setFile(bscan_path);
        if (bscan_file.exists() && bscan_file.isFile()) {
            ui->lineEdit_bscan->setText(bscan_file.canonicalFilePath());
        }
    }
}
/*
MainWindow::~MainWindow()
{
    delete ui;
}
*/

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("Koc University", "Prometheus");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("fireprog_path", fireprog_path);
    settings.setValue("bitfile_path", bitfile_path);
    settings.setValue("bscan_path", bscan_path);
	//event->ignore();
    delete ui;
}

void MainWindow::setDefaultConsoleColor(){
    // Set background color to black, and text to white
    ui->textEdit->setStyleSheet("QTextEdit { background-color: rgb(0, 0, 0) ;	color: rgb(255, 255, 255);}");
    ui->textEdit->setTextColor(Qt::white);
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}

void MainWindow::procStarted()
{
    //ui->textEdit->append("Started");
}

void MainWindow::procError(QProcess::ProcessError procError)
{
    ui->textEdit->setTextColor(Qt::red);
    ui->textEdit->append(tr("Error!!!"));
    ui->textEdit->append(proc->errorString());

    switch (procError) {
      case QProcess::FailedToStart:
        ui->textEdit->append(tr("Failed to start"));
        break;
      case QProcess::Crashed:
        ui->textEdit->append(tr("Crashed"));
        break;
    case QProcess::Timedout:
        ui->textEdit->append(tr("Timedout"));
        break;
    case QProcess::UnknownError:
        ui->textEdit->append(tr("Unknown Error"));
      default:
        ui->textEdit->append(tr("REALLY! Unknown Error"));
      }

    ui->textEdit->ensureCursorVisible();
    setDefaultConsoleColor();
}

void MainWindow::procExited(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->textEdit->append("Done.");
    ui->textEdit->ensureCursorVisible();
}

void MainWindow::progStandardOutput()
{
    QString abc = proc->readAllStandardOutput();
    ui->textEdit->append(abc);
    ui->textEdit->ensureCursorVisible();

}

void MainWindow::progStandardError()
{
    QString abc = proc->readAllStandardError();
    ui->textEdit->append(abc);
    ui->textEdit->ensureCursorVisible();
}

void MainWindow::on_toolBtnProg_clicked()
{
    fireprog_path = QFileDialog::getOpenFileName(this, tr("Select the fireprog file"),"./","fireprog (fireprog fireprog.exe)");
    if (!fireprog_path.isEmpty())
        ui->lineEdit_fireprog->setText(fireprog_path);
}

void MainWindow::on_toolBtnBit_clicked()
{
    bitfile_path = QFileDialog::getOpenFileName(this, tr("Select the bit file"),"./",tr("bit Files (*.bit)"));
    if (!bitfile_path.isEmpty())
        ui->lineEdit_bitfile->setText(bitfile_path);
}

void MainWindow::on_toolBtnBscan_clicked()
{
    bscan_path = QFileDialog::getOpenFileName(this, tr("Select the BSCAN SPI \".bit\" file"),"./",tr("bit Files (*.bit)"));
    if (!bscan_path.isEmpty())
        ui->lineEdit_bscan->setText(bscan_path);
}

void MainWindow::on_checkBox_details_stateChanged(int status)
{
    // status 0 => Hide, 2 => Show
    if (status == 0) {
        ui->textEdit->setVisible(false);
        // Using this to make it resize when hiding the console
        QMetaObject::invokeMethod(this, "adjustSize", Qt::QueuedConnection);

    } else {
        ui->textEdit->setVisible(true);
    }
}

void MainWindow::on_pushButton_Reconfigure_clicked()
{
    ui->textEdit->clear();
    if (ui->lineEdit_fireprog->text().isEmpty()) {
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("ERROR: Select the fireprog path first."));
        setDefaultConsoleColor();
    } else {
        QString program = ui->lineEdit_fireprog->text();
        QStringList arguments;
		arguments.append("-v");
		arguments.append("-r");
		ui->textEdit->setTextColor( QColor( "blue" ) );
		ui->textEdit->setPlainText(program+" "+arguments.join(" "));
		ui->textEdit->setTextColor( QColor( "white" ) );
        proc->start(program, arguments);

    }
}

void MainWindow::on_pushButton_DetectChain_clicked()
{
    ui->textEdit->clear();
    if (ui->lineEdit_fireprog->text().isEmpty()) {
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("ERROR: Select the fireprog path first."));
        setDefaultConsoleColor();
    } else {
        QString program = ui->lineEdit_fireprog->text();
        QStringList arguments;
		arguments.append("-v");
        arguments.append("-j");
		ui->textEdit->setTextColor( QColor( "blue" ) );
		ui->textEdit->setPlainText(program+" "+arguments.join(" "));
		ui->textEdit->setTextColor( QColor( "white" ) );
        proc->start(program, arguments);

    }
}

void MainWindow::on_pushButton_ISC_Values_clicked()
{
    ui->textEdit->clear();
    if (ui->lineEdit_fireprog->text().isEmpty()) {
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("ERROR: Select the fireprog path first."));
        setDefaultConsoleColor();
    } else {
        QString program = ui->lineEdit_fireprog->text();
        QStringList arguments;
		arguments.append("-v");
        arguments.append("-c");
		ui->textEdit->setTextColor( QColor( "blue" ) );
		ui->textEdit->setPlainText(program+" "+arguments.join(" "));
		ui->textEdit->setTextColor( QColor( "white" ) );
        proc->start(program, arguments);

    }
}

void MainWindow::on_pushButton_STAT_Register_clicked()
{
    ui->textEdit->clear();
    if (ui->lineEdit_fireprog->text().isEmpty()) {
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("ERROR: Select the fireprog path first."));
        setDefaultConsoleColor();
    } else {
        QString program = ui->lineEdit_fireprog->text();
        QStringList arguments;
		arguments.append("-v");
        arguments.append("-C");
		ui->textEdit->setTextColor( QColor( "blue" ) );
		ui->textEdit->setPlainText(program+" "+arguments.join(" "));
		ui->textEdit->setTextColor( QColor( "white" ) );
        proc->start(program, arguments);

    }
}

void MainWindow::on_pushButton_ProgramFPGA_clicked()
{
	ui->textEdit->clear();
    if (ui->lineEdit_fireprog->text().isEmpty()) {
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("ERROR: Select the fireprog path first."));
        setDefaultConsoleColor();
    } else if (ui->lineEdit_bitfile->text().isEmpty()) {
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("ERROR: Select the \".bit\" file first."));
        setDefaultConsoleColor();
    } else {
        QString program = ui->lineEdit_fireprog->text();
        QStringList arguments;
        arguments.append("-v");
        arguments.append("-f");
        arguments.append(ui->lineEdit_bitfile->text());
		ui->textEdit->setTextColor( QColor( "blue" ) );
		ui->textEdit->setPlainText(program+" "+arguments.join(" "));
		ui->textEdit->setTextColor( QColor( "white" ) );
        proc->start(program, arguments);
    }
}

void MainWindow::on_pushButton_ProgramFlash_clicked()
{
	ui->textEdit->clear();
    if (ui->lineEdit_fireprog->text().isEmpty()) {
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("ERROR: Select the fireprog path first."));
        setDefaultConsoleColor();
    } else if (ui->lineEdit_bscan->text().isEmpty()) {
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("ERROR: Select the BSCAN SPI \".bit\" file first."));
        setDefaultConsoleColor();
    } else if (ui->lineEdit_bitfile->text().isEmpty()) {
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("ERROR: Select the \".bit\" file first."));
        setDefaultConsoleColor();
    } else {
        QString program = ui->lineEdit_fireprog->text();
        QStringList arguments;
        arguments.append("-v");
		arguments.append("-b");
        arguments.append(ui->lineEdit_bscan->text());
		arguments.append("-f");
        arguments.append(ui->lineEdit_bitfile->text());
		arguments.append("-r");
		ui->textEdit->setTextColor( QColor( "blue" ) );
		ui->textEdit->setPlainText(program+" "+arguments.join(" "));
		ui->textEdit->setTextColor( QColor( "white" ) );
        proc->start(program, arguments);
    }
}


void MainWindow::on_actionAbout_triggered()
{
    QString myTitle = "About";
    QString myBody = trUtf8("<span style=\"font-family: serif; font-size:16pt; font-weight:600; color:#aa0000;\">Prometheus</span><br><br>"
                        "A simple graphical user interface for 'fireprog' which can program Prometheus FPGA boards.<br><br>"
                        "Developed by: Altynbek Isabekov and Onurhan Öztürk<br>"
						"Copyright (C) 2017"
                        );
    QMessageBox::about(this, myTitle, myBody);
}
