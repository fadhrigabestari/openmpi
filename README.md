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
Solusi paralel yang kami gunakan ialah menggunakan MPI_Scatter dan MPI_Gather untuk radix sort. MPI_Scatter digunakan untuk menyebar data dari satu task ke setiap task lainnya. Kami mengimplementasikan MPI_Scatter untuk algoritma pemberian *flag* pada array acak.
<br>
Lalu kami juga mengimplementasikan MPI_Scatter untuk
### Analisis Solusi
### Jumlah *Thread* yang Digunakan
Jumlah *thread* yang kami gunakan adalah 4 karena sesuai dengan core yang dialokasikan pada Virtual Machine yang kami gunakan untuk menjalankan program.
### Pengukuran Kinerja
### Analisis Perbandingan Kinerja Serial dan Paralel
