# Wood-and-Steel

Video playtest version 1.3: https://youtu.be/GYRbnVeZqf8

1. Các chức năng đã hoàn thành được

Menu bao gồm những lựa chọn như chơi mới, hướng dẫn chơi hoặc thoát.
Quân cờ được thể hiện hoàn thiện bằng đồ hoạ, có hiệu ứng di chuyển.
Có thể chơi lại nhiều ván liên tiếp.

2. Các kỹ thuật lập trình đã sử dụng (thư viện, thuật toán)

SFML được sử dụng. Kỹ thuật chia chương trình thành nhiều hàm.


Thuật toán đo khoảng cách các bước trên bàn cờ hexagon.
Thuật toán kiểm tra con trỏ chuột trên một bàn cờ hexagon.


3. Những điểm tâm đắc trong khi thực hiện dự án

Đây là một trò chơi do em thiết kế chứ không mô phỏng bất kì một trò chơi nào. 
Tuy luật chơi và cách chơi vẫn chưa được chau chuốt như em mong đợi nhưng em rất tâm đắc với các thuật toán do em viết trong trò chơi, đặc biệt là thuật toán kiểm tra con trỏ chuột và đo khoảng cách các bước trên bàn cờ hexagon.


LUẬT CHƠI

1. Trò chơi dành cho hai người, mỗi người chơi điều khiển một phe, Wood (phe bên phải) và Steel (phe bên trái).

2. Mỗi lượt một phe có 3 lượt hành động. Có thể dùng lượt hành động để:
Di chuyển đến kẻ địch sẽ tiêu diệt quân địch đó. 
Hoặc Khai thác tài nguyên bằng cách click chuột vào những ô màu vàng (đối với Steel) & xanh (đối với Wood).

3. Sử dụng tài nguyên khai thác được để tạo ra những lá bài hỗ trợ (click chuột vào ô số tài nguyên của phe mình phía trên bản đồ để tạo). Hiện tại vì là demo nên mới chỉ có một lá bài tăng số bước lên 2.

4. Steel chiến thắng bằng cách đi đến ô màu đỏ, 
Wood chiến thắng bằng cách tiêu diệt toàn bộ quân cờ phe Steel.

5. [ Chưa có ] Phe Wood có khả năng hồi sinh vô tận mỗi khi bị tiêu diệt, phe Steel có thể sử dụng tài nguyên để tạo thêm quân lính. 
Tính năng này vẫn đang trong giai đoạn hoàn thiện để mang tính cân bằng cho trò chơi.

HƯỚNG DẪN
1. Click chuột trái vào quân cờ để hiện những nước di chuyển.
2. Click vào những ô di chuyển (những ô có xuất hiện một hexagon nhỏ màu đen) để thực hiện hành động.
3. Ấn Esc bất kì lúc nào khi đang chơi để thoát ra ngoài menu.
