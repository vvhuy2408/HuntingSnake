Victory frame: 
- Xuất hiện khi người chơi thắng, sẽ có hai nút là accept và no, thì hai nút đó ở trong Design/Assets/Button/confirm.png và Design/Assets/exit.png
- Thì: thông số x, y của nút và frame được để sẵn trong effects.cpp rồi
- Người dùng ấn acpt thì sẽ quay về main menu (sử dụng ChangeScreen trong ScreenState.cpp)
- Người dùng ấn no acpt thì không đổi gì

Tương tự vậy với Lose frane:
- người dùng ấn acpt thì quay lại vòng chơi gần nhất
- người dùng ấn no acpt thì quay về main menu

Hiệu ứng Save Game:
- người dùng ấn Ctrl + S -> hiện ra Save Game.

T có làm hàm r, chỉ cần gắn vô main th nhé. Làm tương tự cái isExit với isDelete á. Còn thiếu logic sơ bộ khi delete (xoá một slot), rename(đổi tên slot), lose(khi người chơi thua và muốn quay về màn chơi gần nhất). 
Logic Win:

Progress bar: T chia làm 6 cái ảnh để tương ứng với 6 tiến độ điểm trong một vòng chơi nha. T có đánh stt rồi từ nhỏ đến lớn, trong Design/Assets

Về font cho time và level: T đã tạo sẵn và chỉnh xong kích thước, màu, position rồi. Chỉ còn thiếu cái áp font lên thời gian chơi á.