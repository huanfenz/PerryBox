#include "crc_calculator_page.h"
#include "ui_crc_calculator_page.h"
#include "crc_calculator.h"
#include <QPushButton>
#include <fmt/format.h>

CrcCalculatorPage::CrcCalculatorPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CrcCalculatorPage)
{
    ui->setupUi(this);

    // 用参数模型表填充下拉框
    for (const CrcModel& model : crcModels()) {
        ui->cbox_crc_model->addItem(QString::fromUtf8(model.name));
    }

    // 计算按钮事件
    connect(ui->btn_crc_calc, &QPushButton::clicked, this, [this](){
        const auto& models = crcModels();
        int index = ui->cbox_crc_model->currentIndex();
        if (index < 0 || index >= static_cast<int>(models.size())) {
            return;
        }
        const CrcModel& model = models[index];

        std::string inputStr = ui->pedit_crc_input->toPlainText().toStdString();
        if (!isValidBaseStr(inputStr, BaseEnum::HEX)) {
            ui->label_info_crc->setText("十六进制格式错误，请检查");
            ui->ledit_crc_result->clear();
            return;
        }
        ui->label_info_crc->setText("");
        std::vector<uint8_t> inputNums = baseStr2Nums(inputStr, BaseEnum::HEX);

        uint64_t value = crcCalc(model, inputNums);
        ui->ledit_crc_result->setText(QString::fromStdString(
            fmt::format("{:0{}X}", value, model.width / 4)));
    });

    // 清空按钮事件
    connect(ui->btn_crc_clear, &QPushButton::clicked, this, [this](){
        ui->pedit_crc_input->clear();
        ui->ledit_crc_result->clear();
        ui->label_info_crc->setText("");
    });
}

CrcCalculatorPage::~CrcCalculatorPage()
{
    delete ui;
}
