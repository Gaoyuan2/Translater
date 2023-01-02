#include "widget.h"
#include "ui_widget.h"
#include"curl/curl.h"
#include "json/json.h"
#include "access_token.h"
#include<string>
#include<iostream>

#include<QTextCodec>
#include<QFile>
#include <QtDebug>
#include <QTime>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QList>

#include <cstdlib>
#include <ctime>

const static std::string request_url = "https://aip.baidubce.com/rpc/2.0/mt/texttrans/v1";

const std::string access_token_url = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials";


// 以下为给初始成员变量赋初值，使之在构造时赋值
Widget::Widget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Widget)
{
	ui->setupUi(this);

	//setWindowTitle("AI");
}

Widget::~Widget()
{
	delete ui;
}

/**
 * 用以获取access_token的函数，使用时需要先在百度云控制台申请相应功能的应用，获得对应的API Key和Secret Key
 * @param access_token 获取得到的access token，调用函数时需传入该参数
 * @param AK 应用的API key
 * @param SK 应用的Secret key
 * @return 返回0代表获取access token成功，其他返回值代表获取失败
 */

static size_t callback(void *ptr, size_t size, size_t nmemb, void *stream) {
	// 获取到的body存放在ptr中，先将其转换为string格式
	std::string s((char *)ptr, size * nmemb);
	// 开始获取json中的access token项目
	Json::Reader reader;
	Json::Value root;
	// 使用boost库解析json
	reader.parse(s, root);
	std::string* access_token_result = static_cast<std::string*>(stream);
	*access_token_result = root["access_token"].asString();
	return size * nmemb;
}

int get_access_token(std::string &access_token, const std::string &AK, const std::string &SK) {
	CURL *curl;
	CURLcode result_code;
	int error_code = 0;
	curl = curl_easy_init();
	if (curl) {
		std::string url = access_token_url + "&client_id=" + AK + "&client_secret=" + SK;
		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		std::string access_token_result;
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &access_token_result);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
		result_code = curl_easy_perform(curl);
		if (result_code != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(result_code));
			return 1;
		}
		access_token = access_token_result;
		curl_easy_cleanup(curl);
		error_code = 0;
	}
	else {
		fprintf(stderr, "curl_easy_init() failed.");
		error_code = 1;
	}
	return error_code;
}

int textTrans(QString &json_result, const std::string &access_token,QString s) {
	std::string url = request_url + "?access_token=" + access_token;
	CURL *curl = NULL;
	CURLcode result_code;
	int is_success;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		std::string  name1 ="{\"from\":\"auto\",\"to\":\"en\",\"q\":";
		std::string  name2 = ", \"termIds\":\"\"}";
		std::string  name = name1+ "\"" +s.toStdString()+"\""+ name2;
		std::cout << name;
		const char *data = const_cast<char*>(name.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	
		//result_code = curl_easy_perform(curl);
		FILE *outfile;
		outfile = fopen("nihao.txt", "wb");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, outfile);
		result_code = curl_easy_perform(curl);
		fclose(outfile);


		QFile file("nihao.txt");
		QTextStream in(&file);
		file.open(QIODevice::ReadOnly);
		in.setCodec("UTF-8"); // 设置文件的编码格式为UTF-8
		json_result = in.readLine(); // 此时以UTF-8的方式从文件中读取内容，结果正常
		file.close();

		curl_easy_cleanup(curl);
		is_success = 0;
	}
	else {
		fprintf(stderr, "curl_easy_init() failed.");
		is_success = 1;
	}
	return is_success;
}

QString  result(QString value) {
	//{"result":{"from":"en","trans_result":[{"dst":"你好","src":"hello"}],"to":"zh"},"log_id":1608501655409320514}
	
	QJsonDocument jsonDoc = QJsonDocument::fromJson(value.toUtf8());
	QJsonObject jsonObj = jsonDoc.object();

	QJsonObject resultObj = jsonObj["result"].toObject();
	QString from = resultObj["from"].toString();
	QString to = resultObj["to"].toString();

	QJsonArray transResultArr = resultObj["trans_result"].toArray();
	QJsonObject transResultObj = transResultArr[0].toObject();
	QString src = transResultObj["src"].toString();
	QString dst = transResultObj["dst"].toString();

	qint64 logId = jsonObj["log_id"].toVariant().toLongLong();

	return dst;
}

void Widget::on_pushButton_send_clicked()
{
	//从文本框获取输入数据
	QString inputStr = ui->textEdit_input->toPlainText().toUtf8();
	//BUG： 解决输入字符跨行后 只翻译第一行的问题
	inputStr.replace(QString("\n"), QString(""));
	qDebug() << inputStr;
	//判断为翻译模式还是论文模式
	// 翻译模式 为直接翻译成指定的语言，翻译次数一次
	// 论文模式 按照指定的翻译模式翻译，翻译次数为多次
	qDebug() <<"Fanyimoshi";
	//m_transResult = getSyncData(getSplicingData(inputStr));


	std::string access_token;
	int a = get_access_token(access_token, "Your Ak", "Your SK");
	qDebug() << "get_access" << a;
	QString json_result;

	a = textTrans(json_result, access_token, inputStr);
	qDebug() << "textTrans" << a;
	qDebug() << "json::::" << json_result;

	//下面为针对百度api返回的json格式进行解析
		
	//json_result.remove('\\');
	qDebug().noquote() << "移除后::::" << json_result;
	m_transResult = result(json_result);

	// 更新结果显示
	updateResult();
}
void Widget::updateResult() {
	// 更新指定数据到文本输出框
	ui->textEdit_output->setPlainText(m_transResult);
	qDebug() << m_transResult;
}

