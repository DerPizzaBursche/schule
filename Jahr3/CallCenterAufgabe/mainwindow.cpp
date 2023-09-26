#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <kunde.h>
#include <QVector>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Kunde kunde("","","");
    this->setTextLabelsAndNavigationButtons(kunde);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_Neu_triggered()
{
    ui->lineEditNachname->clear();
    ui->lineEditVorname->clear();
    ui->lineEditTelefonnummer->clear();
}

void MainWindow::on_pushButtonAdd_clicked()
{
    QString nName = ui->lineEditNachname->text();
    QString vName = ui->lineEditVorname->text();
    QString tel = ui->lineEditTelefonnummer->text();
    Kunde NeuerKunde(vName,nName,tel);
    if(int index = Kunden.indexOf(NeuerKunde) == -1){
        Kunden.push_back(NeuerKunde);
    }else{
        //Kunden[index] = NeuerKunde;
        QMessageBox::information(this, "Kunde Bereits vorhanden","Der Kunde ist bereits vorhanden!");
    }
    this->setTextLabelsAndNavigationButtons(NeuerKunde);
}

void MainWindow::on_action_Speichern_triggered()
{
    QString KundenToSave = this->ToJson(Kunden);
    QString saveFileName = QFileDialog().getSaveFileName();

    if(saveFileName == "" || saveFileName == NULL){
        saveFileName = default_file;
    }
    QFile file(saveFileName);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << KundenToSave;
        file.close();

    }
    else
    {
        QMessageBox::warning(this,"Fehler","Fehler beim Speichern!");
    }
    if(saveFileName == default_file){
        QMessageBox::information(this, "Speichern erfolgreich","Kontakte wurden lokal gespeichert");
    }else{
        QMessageBox::information(this, "Speichern erfolgreich","Kontakte wurden in Der Datei: " + saveFileName + " gespeichert");
    }
}
void MainWindow::on_action_Laden_triggered()
{
    QString loadFileName = QFileDialog().getOpenFileName();

    if(loadFileName == "" || loadFileName == NULL){
        loadFileName = default_file;
    }
    QFile file(loadFileName);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString kundenAsString = in.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(kundenAsString.toUtf8());
        QJsonArray kundenAsJsonArray = doc.array();

        for(QJsonValue kunde : kundenAsJsonArray){
            if(kunde.isObject()){
                QJsonObject tmpKunde = kunde.toObject();
                Kunde kundeToObjet;
                kundeToObjet.fromQJsonObject(tmpKunde);
                Kunden.push_back(kundeToObjet);
            }
        }
        this->setTextLabelsAndNavigationButtons(Kunden.first());
    }
    else
    {
        QMessageBox::warning(this,"Fehler","Fehler beim Öffnen der Datei!");
    }
}

QString MainWindow::ToJson(QVector<Kunde>Kunden){
    QJsonArray KundenArray;
    for (Kunde kunde : Kunden) {
        KundenArray.append(kunde.toQJsonObject());
    }
    QJsonObject rootKundenarray;
    rootKundenarray["Kunden"] = KundenArray;
    QJsonDocument doc;
    doc.setArray(KundenArray);
    QString JSON(doc.toJson());
    return JSON;
}

void MainWindow::setTextLabelsAndNavigationButtons(Kunde kunde){
    ui->lineEditNachname->setText(kunde.GetNachname());
    ui->lineEditVorname->setText(kunde.GetVorname());
    ui->lineEditTelefonnummer->setText(kunde.GetTel());
    QString maxCount =  QString::number(Kunden.size());
    int index = Kunden.indexOf(kunde);
    if (index != -1) {
        QString IndexAsString = QString::number(index +1);

        ui->labelAnzahl->setText("Eintrag " + IndexAsString  + " von " + maxCount);

        ui->pushButtonZurueck->setEnabled(index > 0);
        ui->pushButtonWeiter->setEnabled(index < Kunden.size() - 1);
    } else {
        ui->labelAnzahl->setText("Eintrag 0 von " + maxCount);
        ui->pushButtonZurueck->setEnabled(0);
        ui->pushButtonWeiter->setEnabled(0);
    }
}




void MainWindow::on_pushButtonZurueck_clicked()
{
    QString nName = ui->lineEditNachname->text();
    QString vName = ui->lineEditVorname->text();
    QString tel = ui->lineEditTelefonnummer->text();
    Kunde tmp(vName, nName, tel);
    int currentIndex = Kunden.indexOf(tmp);
    this->setTextLabelsAndNavigationButtons(Kunden[currentIndex - 1]);
}


void MainWindow::on_pushButtonWeiter_clicked()
{
    QString nName = ui->lineEditNachname->text();
    QString vName = ui->lineEditVorname->text();
    QString tel = ui->lineEditTelefonnummer->text();
    Kunde tmp(vName, nName, tel);
    int currentIndex = Kunden.indexOf(tmp);
    this->setTextLabelsAndNavigationButtons(Kunden[currentIndex + 1]);
}


void MainWindow::on_action_Beenden_triggered()
{
    QMessageBox::information(this, "Programm wird beendet","Alle ungespeicherten Änderungen werden gelöscht");
    QApplication::quit();
}
