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


// ����Ϊ����ʼ��Ա��������ֵ��ʹ֮�ڹ���ʱ��ֵ
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
 * ���Ի�ȡaccess_token�ĺ�����ʹ��ʱ��Ҫ���ڰٶ��ƿ���̨������Ӧ���ܵ�Ӧ�ã���ö�Ӧ��API Key��Secret Key
 * @param access_token ��ȡ�õ���access token�����ú���ʱ�贫��ò���
 * @param AK Ӧ�õ�API key
 * @param SK Ӧ�õ�Secret key
 * @return ����0�����ȡaccess token�ɹ�����������ֵ�����ȡʧ��
 */

static size_t callback(void *ptr, size_t size, size_t nmemb, void *stream) {
	// ��ȡ����body�����ptr�У��Ƚ���ת��Ϊstring��ʽ
	std::string s((char *)ptr, size * nmemb);
	// ��ʼ��ȡjson�е�access token��Ŀ
	Json::Reader reader;
	Json::Value root;
	// ʹ��boost�����json
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
		in.setCodec("UTF-8"); // �����ļ��ı����ʽΪUTF-8
		json_result = in.readLine(); // ��ʱ��UTF-8�ķ�ʽ���ļ��ж�ȡ���ݣ��������
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
	//{"result":{"from":"en","trans_result":[{"dst":"���","src":"hello"}],"to":"zh"},"log_id":1608501655409320514}
	
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
	//���ı����ȡ��������
	QString inputStr = ui->textEdit_input->toPlainText().toUtf8();
	//BUG�� ��������ַ����к� ֻ�����һ�е�����
	inputStr.replace(QString("\n"), QString(""));
	qDebug() << inputStr;
	//�ж�Ϊ����ģʽ��������ģʽ
	// ����ģʽ Ϊֱ�ӷ����ָ�������ԣ��������һ��
	// ����ģʽ ����ָ���ķ���ģʽ���룬�������Ϊ���
	qDebug() <<"Fanyimoshi";
	//m_transResult = getSyncData(getSplicingData(inputStr));


	std::string access_token;
	int a = get_access_token(access_token, "Your Ak", "Your SK");
	qDebug() << "get_access" << a;
	QString json_result;

	a = textTrans(json_result, access_token, inputStr);
	qDebug() << "textTrans" << a;
	qDebug() << "json::::" << json_result;

	//����Ϊ��԰ٶ�api���ص�json��ʽ���н���
		
	//json_result.remove('\\');
	qDebug().noquote() << "�Ƴ���::::" << json_result;
	m_transResult = result(json_result);

	// ���½����ʾ
	updateResult();
}
void Widget::updateResult() {
	// ����ָ�����ݵ��ı������
	ui->textEdit_output->setPlainText(m_transResult);
	qDebug() << m_transResult;
}

