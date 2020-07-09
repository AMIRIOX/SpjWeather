#pragma once

#include <QtWidgets/QWidget>
#include "ui_SpjrSys.h"
#include <qchart.h>
#include <qnetworkrequest.h>
#include <qnetworkaccessmanager.h>
#include <qfile.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qnetworkreply.h>
#include <qfile.h>
#include <iostream>
#include <map>
#include <qmessagebox.h>
using namespace std;
class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

struct baseWeather {
	QString date;
	QString week;
	QString fl;     //����
	QString fx;     //����
	QString type;   //����
	QString temperatureLow;   //�����
	QString temperatureHighl; //�����
	QString notice;
	int aqi;
	QString sunrise;
	QString sunset;
};

struct weather : public baseWeather{
	QString psmwy;   //ʡ��
	QString city;   //����
	QString shidu;
	QString quality;
	QString wendu;
	QString gmzs;
	int pm25;
	int pm10;
};

class SpjrSys : public QWidget
{
	Q_OBJECT

public:
	SpjrSys(QWidget *parent = Q_NULLPTR);

	//����Ҫ��API����Ĳ���,�������Ƶ�
	void setRequest(QNetworkRequest& req, QString city);

	////����ͽ���API���ص�Json�ļ�
	void process(QJsonObject data);

private:
	Ui::SpjrSysClass ui;
	QNetworkAccessManager* manage;
	QNetworkRequest network_request;
	int tmpSize;
	QTimer* timer; //����API���Ƽ��ÿ�����������һ��,����Ҫ���м�ʱ
	bool can = false;//�Ƿ��Ѿ�����3sʱ��,����������(ʵ����������5s�ļ��,�Է���һ)
public:
	~SpjrSys();
	weather today;
	baseWeather futureList[15];
	std::map<std::string, std::string> cities;
	vector<string> getCityInfo();
	vector<string> city_info;
	void refreshData(QString city);
	void setUiFormats();
	int getAvengeAqi();
	int getAvengeLow();
	int getAvengeHigh();
private slots:
	void change();
	void getReplyFinished(QNetworkReply* rep) {
		QJsonObject jsonDoc = QJsonDocument::fromJson(rep->readAll()).object();
		try {
			process(jsonDoc);
			setUiFormats();
		}
		catch (...) {
			QMessageBox::critical(nullptr, u8"��ʾ", u8"�޷���ȡ������Ϣ!", QMessageBox::Ok);
		}
	}
	void getInputCity(bool clicked);
};
