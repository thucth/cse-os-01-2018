## Bí ẩn của sbrk()?

#### 1. Giá trị sbrk(0) ban đầu khác biệt với các lần sau
```c
printf("1. 0: %lu\n", (uintptr_t) sbrk(0));
printf("2. 0: %lu\n", (uintptr_t) sbrk(0));
printf("3. 5: %lu\n", (uintptr_t) sbrk(5));
printf("4. 0: %lu\n", (uintptr_t) sbrk(0));
printf("5. 0: %lu\n", (uintptr_t) sbrk(0));
```
Kết quả:
```
1. 0: 23666688
2. 0: 23801856
3. 5: 23801856
4. 0: 23801861
5. 0: 23801861
```
+ Nhận xét: Giá trị ở lần 1 khác với giá trị ở lần 2, còn ở các lần sau thì đúng.
+ Giải thích: Do `sbrk(int size)` tăng/giảm vị trí program break của process và trả về vị trí cũ (tính từ vị trí hiện tại). Ở lần gọi đầu tiên, vùng heap chưa được đã động tới tính từ lúc process start lên, do đó giá trị khởi tạo của program break là của các process khác hoặc giá trị nào đó (*the end of the process's data segment*), do đó khi gọi lần đầu, nó đụng đến vùng heap đồng thời khởi tạo lại program break cho process này.

#### 2. Có gọi sbrk() ở lần đầu nhưng in ra vẫn khác như ví dụ 1
```c
sbrk(0); // 0
sbrk(5); // 1
sbrk(0); // 2

printf("1. 0: %lu\n", (uintptr_t) sbrk(0));
printf("2. 0: %lu\n", (uintptr_t) sbrk(0));
printf("3. 5: %lu\n", (uintptr_t) sbrk(5));
printf("4. 0: %lu\n", (uintptr_t) sbrk(0));
printf("5. 0: %lu\n", (uintptr_t) sbrk(0));
```
Kết quả:
```
1. 0: 20901893
2. 0: 21041152
3. 5: 21041152
4. 0: 21041157
5. 0: 21041157
```
+ Nhận xét: Tại sao gọi `sbrk()` rồi (dù có comment lại dòng 0, 1 hoặc 2) mà dòng 1 vẫn khác như ví dụ 1?
+ Giải thích: khác biệt ở chỗ gọi `printf()`, có `printf()` thì ok, không thì fail. Cái này hình như do `printf()` đã làm gì đó đến vùng heap nên đã khởi tạo program break cho process này. Phải kiểm tra lại ở ví dụ 3


#### 3. Thử `printf()` trước xem có khác biệt?
```c
printf("something here\n");
printf("1. 0: %lu\n", (uintptr_t) sbrk(0));
printf("2. 0: %lu\n", (uintptr_t) sbrk(0));
printf("3. 5: %lu\n", (uintptr_t) sbrk(5));
printf("4. 0: %lu\n", (uintptr_t) sbrk(0));
printf("5. 0: %lu\n", (uintptr_t) sbrk(0));
```
Kết quả:
```
something here
1. 0: 38879232
2. 0: 38879232
3. 5: 38879232
4. 0: 38879237
5. 0: 38879237
```
+ Nhận xét: Ổn rồi, vậy là tại gọi `printf()` trước.
+ Nhưng tại sao? Không biết được! Thử không dùng `printf` mà khởi tạo gì đó trước.

#### 4. Thử khởi tạo biến gì đó trước thay vì `printf`
```c
int t = 7;
printf("1. 0: %lu\n", (uintptr_t) sbrk(0));
printf("2. 0: %lu\n", (uintptr_t) sbrk(0));
printf("3. 5: %lu\n", (uintptr_t) sbrk(5));
printf("4. 0: %lu\n", (uintptr_t) sbrk(0));
printf("5. 0: %lu\n", (uintptr_t) sbrk(0));
```
Kết quả:
```
1. 0: 33574912
2. 0: 33710080
3. 5: 33710080
4. 0: 33710085
5. 0: 33710085
```
+ Nhận xét: fail!
+ Giải thích: có lẽ tại nó không đụng đến vùng heap nên không khởi tạo program break. Vậy phải thử cấp phát động xem sao?

#### 5. Thử cấp phát động thay vì cấp phát tĩnh
```c
int * a = malloc(50 * sizeof(int));
printf("1. 0: %lu\n", (uintptr_t) sbrk(0));
printf("2. 0: %lu\n", (uintptr_t) sbrk(0));
printf("3. 5: %lu\n", (uintptr_t) sbrk(5));
printf("4. 0: %lu\n", (uintptr_t) sbrk(0));
printf("5. 0: %lu\n", (uintptr_t) sbrk(0));
```
Kết quả:
```
1. 0: 7069696
2. 0: 7069696
3. 5: 7069696
4. 0: 7069701
5. 0: 7069701
```
+ Nhận xét: ngon! Vậy là phải cấp phát động trước, để nó đụng đến vùng heap. Từ đó có thể nghĩ ra rằng thằng `printf` có khả năng là đụng đến vùng heap như cấp phát động. Còn tại sao thì chưa biết!

#### 5. Thử dùng `malloc()` vài lần xem có đổi `sbrk()` không?
```c
int * a = malloc(50 * sizeof(int));
printf("1. 0: %lu\n", (uintptr_t) sbrk(0));
printf("2. 0: %lu\n", (uintptr_t) sbrk(0));
printf("3. 5: %lu\n", (uintptr_t) sbrk(5));
printf("4. 0: %lu\n", (uintptr_t) sbrk(0));
printf("5. 0: %lu\n", (uintptr_t) sbrk(0));

int * b = malloc(50 * sizeof(int));
printf("6. 0: %lu\n", (uintptr_t) sbrk(0));
```
Kết quả:
```
1. 0: 24866816
2. 0: 24866816
3. 5: 24866816
4. 0: 24866821
5. 0: 24866821
6. 0: 24866821
```
+ Nhận xét: Ở lần 6 không thấy thay đổi vị trí, `malloc` hình như không đụng tới program break. Không hiểu được!