# Tubes1_CaKrai17

secara garis besar, project ini merupakan implementasi FSM berbasis OOP dalam C++ untuk mengatur alur state robot (IDLE, MOVEMENT, SHOOT, dll). terdapat 3 file:

- `fsm.hpp`: header file, mendeklarasikan class dan enum
- `fsm.cpp`: implementasi logic state dari header file
- `main.cpp`: driver program

# Cara kerjanya

## state diagram

[*] --> INIT
INIT --> IDLE
IDLE --> MOVEMENT: User memilih move
MOVEMENT --> SHOOTING: Setelah 3x move
SHOOTING --> IDLE: Reset moveCount
IDLE --> CALCULATION: User memilih calculate
CALCULATION --> ERROR: Jika belum ada move
ERROR --> STOPPED: Setelah 3x error

alurnya:

- system mulai di state `INIT`
- beralih ke `IDLE`, menunggu input user (1.status, 2.move, 3.shoot, 4.calculate)
- transisi state berdasarkan action:
  > MOVEMENT: hitung gerakan, jika >=3 pindah ke SHOOTING
  > CALCULATION: error jika tidak ada moveCount, jika moveCount >0 pindah ke IDLE
  > ERROR: akan menjadi STOPPED jika terjadi 3 error

# KONSEP OOP YANG DITERAPKAN

untuk project ini terdapat 2 konsep utama OOP yaitu Enkapsulasi dan Abstraksi:

1. Encapsulation (Enkapsulasi)

- menyembunyikan detail internal seperti variabel dll (private) dan hanya menampilkan antarmuka yang diperlukan (public).
- contohnya pada file fsm.hpp terdapat variabel currentState, lastHeartbeat, delay, dll yang dibungkus di class private,
- dan terdapat class public yang memuat function seperti setter, getter, dll yang dapat diakses user.

2. Abstraction (Abstraksi)

- konsep yang memungkinkan programmer menyederhanakan kompleksitas dengan hanya menampilkan fitur penting dan menyembunyikan detail yang tidak perlu.
- contoh dalam kode `roboFSM.start()`, user tidak perlu tau detail kode internalnya.
- simpelnya (dari yang saya tangkep, CMIIW🙏) misal pengen bikin movement robot misal maju, tinggal bikin function maju() yang nantinya dapat dipanggil ketika dibutuhkan tanpa harus menulis kode nya berulang kali.
