# OpenMPI Radix Sort
Dibuat oleh :
 - Yuly Haruka Berliana Gunawan - 13516031
 - M. Fadhriga Bestari - 13516154

## Petunjuk Penggunaan Program
Run this in your terminal or on the server
```
cd openmpi/src
make
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
> N = 5000
<br>
Radix Sort serial : 14803
<br>
Radix Sort Paralel dengan OpenMPI : 156699

> N = 50.000
<br>
Radix Sort serial : 116493
<br>
Radix Sort Paralel dengan OpenMPI : 248837

> N = 100.000
<br>
Radix Sort serial : 232686
<br>
Radix Sort Paralel dengan OpenMPI : 330071

> N = 200.000
<br>
Radix Sort serial : 483184
<br>
Radix Sort Paralel dengan OpenMPI : 535068

> N = 400.000
<br>
Radix Sort serial : 929879
<br>
Radix Sort Paralel dengan OpenMPI : 957559

### Analisis Perbandingan Kinerja Serial dan Paralel
Berdasarkan percobaan dan perbandingan yang telah kami lakukan, didapatkan bahwa program paralel yang kami gunakan MPI_Scatter dan MPI_Gather membutuhkan waktu tambahan yaitu waktu untuk melakukan pembagian dan pengumpulan data dari ke empat task yang kami assign. Sehingga program paralel yang kami buat, akan lebih lambat pada n yang relatif kecil. Tetapi pada percobaan dengan n = 1.000.000, program paralel radix sort yang kami buat, memproses radix sort lebih cepat.
