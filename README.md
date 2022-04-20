# Melpe1200-2400
## Hướng dẫn build code melpe

* Down source trình biên dịch về: [Link](https://drive.google.com/file/d/1j4FnG02kGxLTWbm-wnNh0U_J4BB1c5cz/view?usp=sharing)
* Giải nén và add đường dẫn thư mục *...\MinGW\bin* vào viến môi trường *PATH*. [Link hướng dẫn](https://quantrimang.com/them-bien-moi-truong-trong-windows-174960)
* Đọc hiểu File Makefile
* Build bằng chương trình thực thi autoMake.bat hoặc vào Command Prompt chạy make (make trong thư mục melpe nữa)

## Hướng dẫn chạy

* File data là dữ liệu các mẫu giọng nói với tần số lấy mẫu là 8kHz. 
* Dùng hàm readBin('filename','short') để đọc dữ liệu. Dữ liệu mẫu: [Link](https://drive.google.com/drive/folders/1bHx8Py1nplFig0ZDf40omNmM-pI9yEFE?usp=sharing)
* Dùng lệnh `melpe fileIn.bin fileOut.bin` fileOut.bin chứa dữ liệu giọng nói đã nén và giải nén
* Convert file bin về file âm thanh đuôi '.wav' để chấm PESQ. Dùng lệnh `PESQ +8000 fileIn.wav fileOut.wav`
