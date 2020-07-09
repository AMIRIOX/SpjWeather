#pragma execution_character_set("utf-8") 
#include "SpjrSys.h"
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qnetworkreply.h>
#include <qfile.h>
#include <iostream>
#include <qdebug.h>
#include <qmessagebox.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <qcombobox.h>
#include <qtimer.h>

QString stdStrToQstr(const string str)
{
	return QString::fromLocal8Bit(str.data());
}

string qStrToStdStr(const QString qstr)
{
	QByteArray cdata = qstr.toLocal8Bit();
	return string(cdata);
}

SpjrSys::SpjrSys(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//this->setWindowIcon();
	//this->setFixedSize(963,527);
	city_info = getCityInfo();
	ui.comboBox->clear();
	QStringList strList;
	void(QPushButton:: * clo)(bool cilcked) = &QPushButton::clicked;
	QObject::connect(ui.exitbtn, clo, this, [this] {
		this->close();
		});
	for (int i = 0; i < city_info.size(); i++) {
		strList << stdStrToQstr(city_info[i]);
	}
	ui.comboBox->addItems(strList);
	ui.comboBox->hide();
	ui.pushButton_2->hide();
	//ui.pushButton->hide();
	void(QPushButton:: * showsin)(bool cilcked) = &QPushButton::clicked;
	QObject::connect(ui.pushButton, showsin, this, [this] {
		ui.pushButton->hide();
		ui.pushButton_2->show();
		ui.comboBox->show();
	});

	void(QPushButton::*tmp)(bool cilcked)=&(QPushButton::clicked);
	QObject::connect(ui.pushButton_2,tmp,this,&SpjrSys::getInputCity);

	//connect(ui.comboBox, SIGNAL(currentlndexChanged(const QString & text)), this, SLOT(void getInputCity(const QString & text)));
	manage = new QNetworkAccessManager(this);

	QString cityc = "101010100";
	setRequest(network_request, cityc);

	connect(manage, SIGNAL(finished(QNetworkReply*)), this, SLOT(getReplyFinished(QNetworkReply*)));
	manage->get(network_request);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(change()));
	timer->start(5000);
}
void SpjrSys::setRequest(QNetworkRequest& req, QString city) {
	req.setUrl(QUrl(QString("http://t.weather.sojson.com/api/weather/city/%1").arg(city)));
}
void SpjrSys::change() {
	std::cout << "Timer Changed";
	can = true;
	timer->stop();
}
void SpjrSys::process(QJsonObject data) {
	//std::cout << "\n|--Sigh:--|\n";
	QStringList fInfoList;
	int status = data.value("status").toInt();
	if (status != 200) throw "Failed to get data from API";
	else {
		today.date = data.value("time").toString();
		today.psmwy = data.value("cityInfo").toObject().value("parent").toString();
		today.city = data.value("cityInfo").toObject().value("city").toString();

		today.shidu = data.value("data").toObject().value("shidu").toString();
		today.pm10 = data.value("data").toObject().value("pm10").toInt();
		today.pm25 = data.value("data").toObject().value("pm25").toInt();
		today.wendu = data.value("data").toObject().value("wendu").toString();
		today.quality = data.value("data").toObject().value("quality").toString();
		today.gmzs = data.value("data").toObject().value("ganmao").toString();
		QJsonArray arr = data.value("data").toObject().value("forecast").toArray();
		//std::cout << arr.size() << "\n";
		//futureList = new QString[arr.size()];
		tmpSize = arr.size();
		//std::cout << "before process\n";
		for (int i = 0; i < arr.count(); i++) {
			if (i == 0) {
				today.type = arr.at(0).toObject().value("type").toString();
				today.temperatureHighl = arr.at(0).toObject().value("high").toString();
				today.temperatureLow = arr.at(0).toObject().value("low").toString();
				today.sunrise = arr.at(0).toObject().value("sunrise").toString();
				today.sunset = arr.at(0).toObject().value("sunset").toString();
				today.week = arr.at(0).toObject().value("week").toString();
				today.aqi = arr.at(0).toObject().value("aqi").toInt();
				today.fl = arr.at(0).toObject().value("fl").toString();
				today.fx = arr.at(0).toObject().value("fx").toString();
				today.notice = arr.at(0).toObject().value("notice").toString();
			}
			else {
				futureList[i - 1].type = arr.at(i).toObject().value("type").toString();
				futureList[i - 1].date = arr.at(i).toObject().value("ymd").toString();
				futureList[i - 1].temperatureHighl = arr.at(i).toObject().value("high").toString();
				futureList[i - 1].temperatureLow = arr.at(i).toObject().value("low").toString();
				futureList[i - 1].sunrise = arr.at(i).toObject().value("sunrise").toString();
				futureList[i - 1].sunset = arr.at(i).toObject().value("sunset").toString();
				futureList[i - 1].week = arr.at(i).toObject().value("week").toString();
				futureList[i - 1].aqi = arr.at(i).toObject().value("aqi").toInt();
				futureList[i - 1].fl = arr.at(i).toObject().value("fl").toString();
				futureList[i - 1].fx = arr.at(i).toObject().value("fx").toString();
				futureList[i - 1].notice = arr.at(i).toObject().value("notice").toString();
				
				
				//std::cout << (std::string)(const char*)(tmp.toLocal8Bit()) << "\n";
				/*if (i == 8) {
					std::cout << "\n\n\nstart---";
					std::cout << (std::string)((const char*)future.type.toLocal8Bit());
					std::cout << "---end\n\n\n";
				}*/
			}
		}
	}
	//std::cout << "----------------------------------\n";
	//std::cout << "before process output\n";
	for (int i = 1; i < tmpSize; i++) {
		QString tmp = QString::number(futureList[i - 1].aqi, 10) + ',' + futureList[i - 1].fl + ',' + futureList[i - 1].fx + ',' + futureList[i - 1].notice + ',' + futureList[i - 1].type + ',' + futureList[i - 1].date + ',' + futureList[i - 1].temperatureHighl + ','
			+ futureList[i - 1].temperatureLow + ',' + futureList[i - 1].sunrise + ',' + futureList[i - 1].sunset + ',' + futureList[i - 1].week + '.';
		std::cout << string(tmp.toLocal8Bit())<<"|"<<'\n';
		std::cout << "<|";
	}
	//std::cout << "wtf???";
	//std::cout << "process ouput is ok\n";
}
vector<string> SpjrSys::getCityInfo() {
	QFile loadFile("city.json");
	if (!loadFile.open(QIODevice::ReadOnly))
	{
		qDebug() << "could't open projects json";
		return vector<string>();
	}
	QByteArray allData = loadFile.readAll();
	loadFile.close();

	QJsonParseError json_error;
	QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

	if (json_error.error != QJsonParseError::NoError)
	{
		qDebug() << "json error!";
		return vector<string>();
	}
	using namespace std;
	QJsonObject jsondata = jsonDoc.object();
	QJsonArray jsonarr = jsondata.value("city").toArray();
	/*std::vector<string> cityName(std::string(), 37);
	std::vector<string> cityCode(std::string(), 37);*/
	vector<string> cityName(37);
	vector<string> cityCode(37);

	for (int i = 0; i < jsonarr.count(); i++) {
		cityName[i] += ((const char*)jsonarr.at(i).toObject().value("city_name").toString().toLocal8Bit()/*.toUtf8().data()*/);
		cityCode[i] += ((const char*)jsonarr.at(i).toObject().value("city_code").toString().toLocal8Bit()/*.toUtf8().data()*/);
		//cityName[i].pop_back();
		//cityName[i] += '\0', cityCode[i] += '\0';
		cities.insert(std::make_pair(cityName[i].c_str(), cityCode[i].c_str()));
		//std::cout << cities[cityName.c_str()]<<'\n';
		//std::cout << cityName << "----";
		//qDebug() <<"|"<< cities[cityName]<<"|";
	}
	/*for (int i = 0; i < cityName[0].size(); i++) {
		std::cout << cityName[0][i]<<'-';
	}*/
	//std::cout<<'-' << cityName[0]<<'-';

	//std::cout << cities["北京"];

	//---------It's good : std::cout << cities[string(QString("北京").toLocal8Bit())];


	//std::cout << cities[string("北京")];


	//std::cout << cities[cityName[0].c_str()] << '\n';
	//std::cout << strlen(cityName[0].c_str())<<"\n";
	//std::cout << cityName[0]<<"--";
	//std::cout << cities["北京"];

	//std::cout <<"--"<< cityName[0]<<"--";
	//std::cout << (cityName[0] == "北京 ");
	//std::cout << (cityName[0].c_str()=="北京\0");

	//std::cout << cities["北京 \0"];

	//std::cout << cities[tmp.c_str()];
	//qDebug() << cities["北京"];
	//qDebug() << cities.count(QString("上海"));
	/*for (std::map<const char *, const char *>::iterator iv = cities.begin(); iv != cities.end(); iv++) {
		std::cout << iv->first <<"--"<< iv->second;
	}*/
	/*qDebug() << cities["北京"];*/
	//QJsonObject jsondata = jsonDoc.object();
	//QJsonArray cityList = jsondata.value("city").toArray();
	//qDebug() << cityList.at(0).toObject().value("city_name").toString();
	//for (int i = 0; i < cityList.count(); i++) {
	//	QString cityCode = cityList.at(i).toObject().value("city_code").toString();
	//	//cityCode.replace(QString('"'), QString(""));
	//	if (!cityCode.isEmpty()) {
	//		//std::cout << "find" << i << endl;
	//		QString cityName = cityList.at(i).toObject().value("city_name").toString();
	//		//cityName.replace(QString('"'), QString(""));
	//		cities.insert(std::pair<const char*,const char*>((const char*)(cityCode.toLocal8Bit()), (const char*)(cityName.toLocal8Bit())));
	//		std::cout << (const char*)(cityList.at(i).toObject().value("city_name").toString().toLocal8Bit());
	//		if(((const char*)(cityCode.toLocal8Bit())) =="北京")
	//			QMessageBox::warning(nullptr,"hhh","hhh", QMessageBox::Yes);
	//	}
	//}
	/*for (std::map<QString, QString>::iterator iv = cities.begin(); iv != cities.end(); iv++) {
		qDebug() << iv->first<<" "<< iv->second;
	}*/
	//qDebug() << cities["\"北京\""] <<"wtf";
	//std::cout << ((std::string)((const char*)(cities["101030100"].toLocal8Bit())));
	/*std::cout << std::string((const char*)jsondata.value("message").toString().toLocal8Bit());
	std::cout << std::string(jsondata.value("data").toString().toLocal8Bit());
	QJsonObject tmp = jsondata.value("data").toObject();
	std::cout << std::string((const char*)tmp.value("ganmao").toString().toLocal8Bit());*/
	return cityName;
}
SpjrSys::~SpjrSys() {
	//std::cout << "???";
	//delete[] futureList;
}

void SpjrSys::getInputCity(bool clicked)
{	
	//if (clicked == false) return;
	if (can == false) {
		QMessageBox::critical(this,u8"错误!" ,u8"您操作频率过快,请每隔5s进行操作!" );
		return;
	}
	QString text = ui.comboBox->currentText();
	std::cout << "I am called: ";
	//string str = qStrToStdStr(text);
	//for (int i = 0; i < 10; i++) {
		//if (city_info[i] == str) {
	QString out = cities[string(text.toLocal8Bit())].c_str();
	std::cout << string(out.toLocal8Bit());
	//QMessageBox::information(nullptr, QString::fromUtf8(string(text.toLocal8Bit()).c_str()), "good", QMessageBox::Ok);
	ui.pushButton->show();
	ui.pushButton_2->hide();
	ui.comboBox->hide();
	refreshData(out);
	can = false;
	timer->start(5000);
	setUiFormats();
	return;
		//}
	//}
	//TODO: 制作前端显示信息
	//setUiFormats();
	QMessageBox::information(nullptr, QString("bad"), QString("bad"), QMessageBox::Ok);
}

void SpjrSys::refreshData(QString city){
	setRequest(network_request, city);
	manage->get(network_request);
}

void SpjrSys::setUiFormats(){
	ui.psmwy->setText(today.psmwy);
	ui.city->setText(today.city);
	ui.gxtime->setText(today.date);
	ui.temp->setText(today.wendu);
	ui.low->setText(today.temperatureLow);
	ui.high->setText(today.temperatureHighl);
	ui.shidu->setText(today.shidu);
	ui.pm25->setText(QString::number(today.pm25,10));
	ui.pm10->setText(QString::number(today.pm10,10));
	ui.qu->setText(today.quality);
	ui.ganmao->setText(today.gmzs);
	ui.type->setText(today.type);
	ui.notice->setText(today.notice);
	ui.aqi->setText(QString::number(today.aqi,10));
	ui.fl->setText(today.fl);
	ui.fx->setText(today.fx);
	ui.sunrise->setText(today.sunrise);
	ui.sunset->setText(today.sunset);
	int tmpAqi = getAvengeAqi();
	int tmpLow = getAvengeLow();
	int tmpHigh = getAvengeHigh();
	ui.aAqi->setText(QString::number(tmpAqi, 10));
	ui.aHigh->setText(QString::number(tmpHigh, 10));
	ui.aLow_t->setText(QString::number(tmpLow, 10));
}
int SpjrSys::getAvengeAqi() {
	int sum = 0;
	for (int i = 1; i < tmpSize; i++) {
		sum += futureList[i-1].aqi;
	}
	return (sum / tmpSize);
}
int SpjrSys::getAvengeLow() {
	int sum = 0;
	for (int i = 1; i < tmpSize; i++) {
		QString tmpStr = futureList[i - 1].temperatureLow;
		tmpStr.replace("低温 ", "");
		tmpStr.replace("℃", "");
		int aqi = tmpStr.toInt();
		sum += aqi;
	}
	return (sum / tmpSize);
}
int SpjrSys::getAvengeHigh() {
	int sum = 0;
	for (int i = 1; i < tmpSize; i++) {
		QString tmpStr = futureList[i - 1].temperatureHighl;
		tmpStr.replace("高温 ", "");
		tmpStr.replace("℃", "");
		int aqi = tmpStr.toInt();
		sum += aqi;
	}
	return (sum / tmpSize);
}