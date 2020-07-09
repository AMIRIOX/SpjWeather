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
	QString fl;     //风力
	QString fx;     //风向
	QString type;   //类型
	QString temperatureLow;   //最低温
	QString temperatureHighl; //最高温
	QString notice;
	int aqi;
	QString sunrise;
	QString sunset;
};

struct weather : public baseWeather{
	QString psmwy;   //省份
	QString city;   //城市
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

	//设置要向API请求的参数,城市名称等
	void setRequest(QNetworkRequest& req, QString city);

	////处理和解析API返回的Json文件
	void process(QJsonObject data);

private:
	Ui::SpjrSysClass ui;
	QNetworkAccessManager* manage;
	QNetworkRequest network_request;
	int tmpSize;
	QTimer* timer; //由于API限制间隔每三秒才能请求一次,所以要进行计时
	bool can = false;//是否已经到了3s时限,可以请求了(实际我是用了5s的间距,以防万一)
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
			QMessageBox::critical(nullptr, u8"提示", u8"无法获取天气信息!", QMessageBox::Ok);
		}
	}
	void getInputCity(bool clicked);
};
