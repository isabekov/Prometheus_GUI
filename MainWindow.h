/* This file is part of miniSProg.

miniSProg is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
any later version.

miniSProg is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with miniSProg.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QFileInfo>
#include <QFileDialog>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_INVOKABLE void adjustSize() { QWidget::adjustSize(); }

public:
    explicit MainWindow(QWidget *parent = 0);
//    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionExit_triggered();

    void procStarted();
    void procError(QProcess::ProcessError procError);
    void procExited(int exitCode, QProcess::ExitStatus exitStatus);
    void progStandardOutput();
    void progStandardError();

    void on_toolBtnProg_clicked();
    void on_toolBtnBscan_clicked();
    void on_toolBtnBit_clicked();

    void on_checkBox_details_stateChanged(int status);

    void on_pushButton_Reconfigure_clicked();
    void on_pushButton_DetectChain_clicked();
    void on_pushButton_Read_DNA_clicked();
    void on_pushButton_ISC_Values_clicked();
    void on_pushButton_STAT_Register_clicked();
	void on_pushButton_ProgramFPGA_clicked();
    void on_pushButton_ProgramFlash_clicked();


    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QProcess * proc;

    void setDefaultConsoleColor();
    QString fireprog_path;
    QString bitfile_path;
    QString bscan_path;
    QFileInfo fireprog_file;
    QFileInfo bit_file;
    QFileInfo bscan_file;
};

#endif // MAINWINDOW_H
