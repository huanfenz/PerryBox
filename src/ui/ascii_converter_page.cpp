#include "ascii_converter_page.h"
#include "ui_ascii_converter_page.h"
#include "byte_warn_highlighter.h"
#include "ascii_converter.h"
#include <QCheckBox>
#include <QPushButton>
#include <map>

using namespace perry;

AsciiConverterPage::AsciiConverterPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AsciiConverterPage)
{
    ui->setupUi(this);

    // 各输入框绑定不可打印字符高亮器
    new HexWarnHighlighter(ui->edit_hex->document());
    new DecWarnHighlighter(ui->edit_dec->document());
    new AsciiWarnHighlighter(ui->edit_ascii->document());
    new BinWarnHighlighter(ui->edit_bin->document());

    // ascii 输入框改变事件
    connect(ui->edit_ascii, &QTextEdit::textChanged, this, [this](){
        handleEditChanged(BaseEnum::ASCII);
    });

    // hex 输入框改变事件
    connect(ui->edit_hex, &QTextEdit::textChanged, this, [this](){
        handleEditChanged(BaseEnum::HEX);
    });

    // dec 输入框改变事件
    connect(ui->edit_dec, &QTextEdit::textChanged, this, [this](){
        handleEditChanged(BaseEnum::DEC);
    });

    // bin 输入框改变事件
    connect(ui->edit_bin, &QTextEdit::textChanged, this, [this](){
        handleEditChanged(BaseEnum::BIN);
    });

    // 重置按钮事件
    connect(ui->btn_reset_ascii, &QPushButton::clicked, this, [this](){
        ui->edit_ascii->setPlainText("");
        ui->edit_hex->setPlainText("");
        ui->edit_dec->setPlainText("");
        ui->edit_bin->setPlainText("");
        ui->label_info_ascii->setText("");
    });

    // 切换十六进制前缀
    connect(ui->cbox_hex_prefix, &QCheckBox::toggled, this, [this](bool){
        handleEditChanged(BaseEnum::ASCII);
    });
}

AsciiConverterPage::~AsciiConverterPage()
{
    delete ui;
}

void AsciiConverterPage::setEditTextNoEvent(QTextEdit* const edit, const QString& text)
{
    edit->blockSignals(true);
    edit->setPlainText(text);
    edit->blockSignals(false);
}

void AsciiConverterPage::handleEditChanged(BaseEnum base)
{
    std::map<BaseEnum, QTextEdit*> updateMap = {
        { BaseEnum::ASCII,  ui->edit_ascii },
        { BaseEnum::HEX,    ui->edit_hex },
        { BaseEnum::DEC,    ui->edit_dec },
        { BaseEnum::BIN,    ui->edit_bin },
    };

    if (updateMap.find(base) == updateMap.end()) {
        return;
    }
    QTextEdit* currEdit = updateMap[base];

    // 从edit获取str，转换器自动转换，valid为false表示校验失败
    std::string str = currEdit->toPlainText().toStdString();
    AsciiConversionResult res = asciiConvert(str, base, ui->cbox_hex_prefix->isChecked());

    if (!res.valid) {
        ui->label_info_ascii->setText("格式错误，请检查");
        return;
    }
    ui->label_info_ascii->setText("");

    // 设置除了自己的其他edit
    std::map<BaseEnum, std::string> resultMap = {
        { BaseEnum::ASCII,  res.ascii },
        { BaseEnum::HEX,    res.hex },
        { BaseEnum::DEC,    res.dec },
        { BaseEnum::BIN,    res.bin },
    };
    for (const auto& pair : updateMap) {
        if (pair.first == base) {
            continue;
        }
        setEditTextNoEvent(pair.second, QString::fromStdString(resultMap[pair.first]));
    }

    // 设置字符长度text
    ui->edit_char_size->setText(QString::number(res.charSize));
}
