# OpenMPI Radix Sort
Dibuat oleh :
 - Yuly Haruka Berliana Gunawan - 13516031
 - M. Fadhriga Bestari - 13516154

## Petunjuk Penggunaan Program
1. Open your terminal from directory that possess this file
2. Type 'make' in your terminal to compile this program
3. Type './radix_sort threads_count' to run the program

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
Lalu kami juga mengimplementasikan MPI_Scatter untuk memberikan

### Analisis Solusi
Menurut kami, penerapan MPI pada radix sort dapat mempercepat sort karena dengan dilakukannya pembagian proses pada setiap task dari core processor, proses pengisian array flag dan proses permutasi yang dilakukan dalam radix sort dipercepat.

### Jumlah *Thread* yang Digunakan
Jumlah *thread* yang kami gunakan adalah 4 karena sesuai dengan core yang dialokasikan pada Virtual Machine yang kami gunakan untuk menjalankan program.
### Pengukuran Kinerja
1. N = 5000
<br>
Radix Sort serial : 19387
<br>
Radix Sort Paralel dengan OpenMPI :

2. N = 50.000
<br>
Radix Sort serial : 321068
<br>
Radix Sort Paralel dengan OpenMPI :

3. N = 100.000
<br>
Radix Sort serial : 780784
<br>
Radix Sort Paralel dengan OpenMPI :

4. N = 200.000
<br>
Radix Sort serial : 1.35732e+06
<br>
Radix Sort Paralel dengan OpenMPI :

5. N = 400.000
<br>
Radix Sort serial : 2.77139e+06
<br>
Radix Sort Paralel dengan OpenMPI :

### Analisis Perbandingan Kinerja Serial dan Paralel
