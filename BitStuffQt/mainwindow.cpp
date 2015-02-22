/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * Website: www.christianbenjaminries.de
 * Source: https://github.com/cbries/utilities
 * License: MIT
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QRegExp>
#include <QWidget>
#include <QString>
#include <QClipboard>
#include <QTextStream>

// STL
#include <bitset>

// ANSI C
#include <stdlib.h>

#define NumberOfBytes 2

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mRecentMode = Dec;
    mRecentValue = 0;
    
    this->changeRadioMode();
    
    QList<QPushButton*> btns = this->findChildren<QPushButton*>(QRegExp("cmdNo[0-9A-F]+"));

    for(auto btn : btns)
    {
        QObject::connect(
                    btn
                  , SIGNAL(clicked())
                  , this
                  , SLOT(handleNumberClicked())
                 );
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isDecMode() { return ui->radioModeDec->isChecked(); }
bool MainWindow::isHexMode() { return ui->radioModeHex->isChecked(); }
bool MainWindow::isBinMode() { return ui->radioModeBinary->isChecked(); }

void MainWindow::handleNumberClicked()
{    
    QString no = sender()->objectName().remove("cmdNo");

    QString currentInput = ui->lineInput->text().trimmed();
    
    currentInput += no;
    
    if(isBinMode())
    {
        int dx = currentInput.length() - (NumberOfBytes * 8);        

        if(dx > 0)
        {
            currentInput.remove(0, abs(dx));
        }        
    }
    
    ui->lineInput->setText(currentInput);

    this->on_lineInput_returnPressed();
}

int MainWindow::currentValueAsDecimal()
{
    int v = 0;
    
    return v;
}

void MainWindow::changeHexState(bool state)
{
    if(state == true)
    {
        this->changeBinState(false);
    }
    
    this->changeDecState(state);
    
    ui->cmdNoA->setEnabled(state);
    ui->cmdNoB->setEnabled(state);
    ui->cmdNoC->setEnabled(state);
    ui->cmdNoD->setEnabled(state);
    ui->cmdNoE->setEnabled(state);
    ui->cmdNoF->setEnabled(state);
}

void MainWindow::changeBinState(bool state)
{
    if(state == true)
    {
        this->changeDecState(false);
        this->changeHexState(false);
    }
    
    ui->cmdNo1->setEnabled(state);
    ui->cmdNo0->setEnabled(state);
}

void MainWindow::changeDecState(bool state)
{
    if(state == true)
    {
        this->changeHexState(false);
        this->changeBinState(false);
    }
    
    ui->cmdNo0->setEnabled(state);
    ui->cmdNo1->setEnabled(state);
    ui->cmdNo2->setEnabled(state);
    ui->cmdNo3->setEnabled(state);
    ui->cmdNo4->setEnabled(state);
    ui->cmdNo5->setEnabled(state);
    ui->cmdNo6->setEnabled(state);
    ui->cmdNo7->setEnabled(state);
    ui->cmdNo8->setEnabled(state);
    ui->cmdNo9->setEnabled(state);
}

void MainWindow::changeRadioMode()
{
    if(isDecMode()) 
    {
        this->changeDecState(true);
    }
    else if(isHexMode())
    {
        this->changeHexState(true);
    }
    else if(isBinMode())
    {
        this->changeBinState(true);
    }
    else
    {
        qDebug() << "Fatal: no mode checked";
    }
}

void MainWindow::on_radioModeDec_clicked()
{
    this->changeRadioMode();
    this->changeValue();
    mRecentMode = Dec;
}

void MainWindow::on_radioModeHex_clicked()
{
    this->changeRadioMode();    
    this->changeValue();
    mRecentMode = Hex;
}

void MainWindow::on_radioModeBinary_clicked()
{
    this->changeRadioMode();    
    this->changeValue();
    mRecentMode = Bin;
}

void MainWindow::changeValue()
{
    switch(mRecentMode)
    {
        case Dec: 
        {
            mRecentValue = ui->lineInput->text().toInt();
        } break;
        
        case Hex: 
        {
            QString v = ui->lineInput->text().trimmed();
            mRecentValue = hex2dec(v);
        } break;
        
        case Bin: 
        {
            QString v = ui->lineInput->text().trimmed();
            mRecentValue = bin2dec(v);
        } break;
    }
    
    if(isDecMode()) 
    {
        ui->lineInput->setText(QString("%1").arg(mRecentValue));        
    }
    else if(isHexMode())
    {
        QString v = dec2hex(mRecentValue);
        ui->lineInput->setText(v.toUpper());
    }
    else if(isBinMode())
    {
        QString v = dec2bin(mRecentValue);
        ui->lineInput->setText(v);
    }
    else
    {
        // ...
    }
}

int MainWindow::bin2dec(const QString &value)
{
    std::bitset<16> bits(value.toStdString().c_str());
    return bits.to_ulong();    
}

QString MainWindow::bin2hex(const QString &value)
{
    return dec2hex(bin2dec(value));
}

QString MainWindow::dec2hex(int value)
{
    char hex[32] = { '\0' };            
    _itoa_s(value, hex, 16);
    QString h(hex);
    //int dx = (NumberOfBytes*2) - h.length();
    //if(dx < 0) { dx = 0; }
    QString suffix="";
    //if(h.length() % 2 == 1) { dx++; }
    //for(int i=0; i < dx; i++) { 
    //    suffix += "0";
    //}
    return QString(suffix + h);
}

QString MainWindow::dec2bin(int value)
{
    std::bitset<16> bin(value);            
    QString binResult = QString("%1").arg(bin.to_string().c_str());    
    return QString(binResult);
}

int MainWindow::hex2dec(const QString &value)
{
    return strtol(value.toStdString().c_str(), 0, 16);
}

QString MainWindow::hex2bin(const QString &value)
{
    return dec2bin(hex2dec(value));
}

void MainWindow::on_cmdCopyDec_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();    
    clipboard->setText(ui->lineResultDec->text().trimmed());
}

void MainWindow::on_cmdcopyHex_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();    
    clipboard->setText(QString("0x%1").arg(ui->lineResultHex->text().trimmed()));
}

void MainWindow::on_cmdCopyBinary_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();    
    clipboard->setText(ui->lineResultBinary->text().trimmed());
}

void MainWindow::calculate0()
{
    if(mRecentValue == 0)
    {
        this->clearResultLines();
        
        return;
    }
    
    ui->lineResultBinary->setText(dec2bin(mRecentValue));
    ui->lineResultDec->setText(QString("%1").arg(mRecentValue));
    ui->lineResultHex->setText(dec2hex(mRecentValue).toUpper());
}

void MainWindow::calculate1()
{
    ui->textOutput->clear();
    
    QString s;
    QTextStream ss(&s);
    
    int v = mRecentValue;
    
    ss << "Bin: " << dec2bin(v) << "\n";
    ss << "Hex (normal): " << dec2hex(v).toUpper() << "\n";
    ss << "Hex (invert): " << dec2hex(~v).toUpper() << "\n";
    ss << "Shifts (left | right):\n";
    ss.setFieldAlignment(QTextStream::AlignRight);
    for(int i=0; i < 16; i++)
    {
        int v_sleft = (v << i);
        int v_sright = (v >> i);
        
        ss.setFieldWidth(2);  ss << i << ": ";
        ss.setFieldWidth(16); ss << dec2bin(v_sleft); 
        ss.setFieldWidth(0);  ss << " | ";
        ss.setFieldWidth(16); ss << dec2bin(v_sright);
        ss.setFieldWidth(0);  ss << "\n";
    }
        
    ui->textOutput->setText(s);
}

void MainWindow::clearResultLines()
{
    ui->lineResultBinary->clear();
    ui->lineResultDec->clear();
    ui->lineResultHex->clear();
}

void MainWindow::on_cmdReset_clicked()
{
    mRecentValue = 0;
    ui->lineInput->clear();
    this->clearResultLines();
}

void MainWindow::on_lineInput_returnPressed()
{
    QString currentInput = ui->lineInput->text().trimmed();
    
    if(isDecMode())      { mRecentValue = currentInput.toInt(); }
    else if(isHexMode()) { mRecentValue = hex2dec(currentInput); }
    else if(isBinMode()) { mRecentValue = bin2dec(currentInput); }
    
    this->calculate0();
    this->calculate1();
}

