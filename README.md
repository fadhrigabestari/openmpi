# OpenMPI Radix Sort
Dibuat oleh :
 - Yuly Haruka Berliana Gunawan - 13516031
 - M. Fadhriga Bestari - 13516154

## Petunjuk Penggunaan Program
1. Open your terminal from directory that possess this file
2. Type 'make' in your terminal to compile this program
3. Type this in your shell
```
mpirun -np 4 radix_sort {n element array}
```

## Pembagian Tugas
- Yuly Haruka Berliana Gunawan - 13516031
	- Parallel the radix sort using OpenMPI
	- Main Program
- M. Fadhriga Bestari - 13516154
	- Parallel the radix sort using OpenMPI
	- Main Program

## Laporan Pengerjaan
### Deskripsi Solusi Paralel
Solusi paralel yang kami gunakan ialah menggunakan MPI_Scatter dan MPI_Gather untuk radix sort. MPI_Scatter digunakan untuk menyebar data dari satu task ke setiap task lainnya. Kami mengimplementasikan MPI_Scatter untuk algoritma pemberian *flag* pada array acak. Setelah menggunakan MPI_Scatter, kami mengambil kembali informasi dari setiap task menggunakan MPI_Gather. Hal itu kami lakukan pada saat program selesai menjalankan algoritma pemberian *flag*.
<br>
Setelah itu, kami menjalankan algoritma untuk mencari index down dan index up tanpa di paralelkan.
<br>
Lalu kami juga mengimplementasikan MPI_Scatter dan MPI_Gather pada proses permutasi untuk mendapatkan should index.

### Analisis Solusi
Menurut kami, penerapan MPI pada radix sort dapat mempercepat sort karena dengan dilakukannya pembagian proses pada setiap task dari core processor, proses pengisian array flag dan proses permutasi yang dilakukan dalam radix sort dipercepat. Tetapi dengan menggunakan MPI_Scatter dan MPI_Gather diperlukan waktu tambahan untuk memecah data ke banyak task dan mengumpulkan data dari berbagai task sehingga proses radix sort untuk n elemen yang relatif kecil akan lebih lambat.

### Jumlah *Thread* yang Digunakan
Jumlah *thread* yang kami gunakan adalah 4 karena sesuai dengan core yang dialokasikan pada Virtual Machine yang kami gunakan untuk menjalankan program.
### Pengukuran Kinerja
1. N = 5000
<br>
Radix Sort serial : 30378
<br>
Radix Sort Paralel dengan OpenMPI : 226287

2. N = 50.000
<br>
Radix Sort serial : 284117
<br>
Radix Sort Paralel dengan OpenMPI : 384187

3. N = 100.000
<br>
Radix Sort serial : 551722
<br>
Radix Sort Paralel dengan OpenMPI : 620323

4. N = 200.000
<br>
Radix Sort serial : 1083420
<br>
Radix Sort Paralel dengan OpenMPI : 1113662

5. N = 400.000
<br>
Radix Sort serial : 2158944
<br>
Radix Sort Paralel dengan OpenMPI : **2053767**

### Analisis Perbandingan Kinerja Serial dan Paralel
Berdasarkan percobaan dan perbandingan yang telah kami lakukan, didapatkan bahwa program paralel yang kami gunakan MPI_Scatter dan MPI_Gather membutuhkan waktu tambahan yaitu waktu untuk melakukan pembagian dan pengumpulan data dari ke empat task yang kami assign. Sehingga program paralel yang kami buat, akan lebih lambat pada n yang relatif kecil. Tetapi dapat dilihat bahwa pada n = 400.000, program paralel radix sort yang kami buat, memproses radix sort lebih cepat.
