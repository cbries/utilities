/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * Website: www.christianbenjaminries.de
 * Source: https://github.com/cbries/utilities
 * License: MIT
 */

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow
    : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    enum Mode { Dec, Hex, Bin };
    Mode mRecentMode;
    int mRecentValue;
    
    bool isDecMode();
    bool isHexMode();
    bool isBinMode();
    
private slots:
    void on_radioModeDec_clicked();    
    void on_radioModeHex_clicked();    
    void on_radioModeBinary_clicked();
    
    void changeHexState(bool state);
    void changeBinState(bool state);
    void changeDecState(bool state);    
    void changeRadioMode();    
    void changeValue();    
    void on_cmdCopyDec_clicked();
    void on_cmdcopyHex_clicked();    
    void on_cmdCopyBinary_clicked();
    
    void handleNumberClicked();
    int currentValueAsDecimal();
    
    void calculate0();
    void calculate1();
        
    void clearResultLines();
    
    int bin2dec(const QString & value);
    QString bin2hex(const QString & value);
    QString dec2hex(int value);
    QString dec2bin(int value); 
    int hex2dec(const QString & value);
    QString hex2bin(const QString & value);
    
    void on_cmdReset_clicked();
    
    void on_lineInput_returnPressed();    
    
private:
    Ui::MainWindow *ui;
};

#endif // __MAINWINDOW_H__
