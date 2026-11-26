// =====================================================================
//  XGhidra - Aplikasi Simulasi Operasi Himpunan (Matematika Diskrit)
//
//  Operasi yang diselesaikan:
//    1. Irisan
//    2. Gabungan
//    3. Selisih
//    4. Beda Setangkup
//
//  Setiap operasi menampilkan: rumus, langkah-langkah, hasil akhir,
//  kardinalitas, dan pengecekan dengan cara lain.
//  Anggota himpunan berupa bilangan bulat dan tidak boleh ganda
//  (sesuai konsep himpunan: distinct objects).
// =====================================================================
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <limits>
#include <cstdlib>
using namespace std;

typedef vector<int> Himpunan;

const int MAKS_Anggota = 20;

// ---------------------------------------------------------------------
//  FUNGSI BANTU TAMPILAN
// ---------------------------------------------------------------------
string angka(int x)
{
    ostringstream o;
    o << x;
    return o.str();
}

string tulis(const Himpunan &H)
{
    if (H.empty()) return "{} (himpunan kosong)";
    string s = "{";
    for (size_t i = 0; i < H.size(); i++) {
        if (i > 0) s += ", ";
        s += angka(H[i]);
    }
    s += "}";
    return s;
}

void judul(const string &teks)
{
    cout << "\n==================================================\n";
    cout << "  " << teks << "\n";
    cout << "==================================================\n";
}

// ---------------------------------------------------------------------
//  FUNGSI INPUT
// ---------------------------------------------------------------------
int bacaAngka(const string &pesan)
{
    int x;
    cout << pesan;
    while (!(cin >> x)) {
        if (cin.eof()) {
            cout << "\nInput berakhir. Program ditutup.\n";
            exit(0);
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input harus berupa bilangan bulat. Coba lagi: ";
    }
    return x;
}

int bacaJumlah(const string &pesan, int maks)
{
    int n = bacaAngka(pesan);
    while (n < 0 || n > maks) {
        n = bacaAngka("Jumlah harus 0 sampai " + angka(maks) + ". Coba lagi: ");
    }
    return n;
}

bool ada(const Himpunan &H, int x)
{
    return find(H.begin(), H.end(), x) != H.end();
}

Himpunan bacaHimpunan(const string &nama)
{
    Himpunan H;
    int n = bacaJumlah("Jumlah Anggota " + nama + " (0-" + angka(MAKS_Anggota) + "): ", MAKS_Anggota);
    while ((int)H.size() < n) {
        int x = bacaAngka("  Anggota ke-" + angka((int)H.size() + 1) + " dari " + nama + ": ");
        if (ada(H, x)) {
            cout << "  -> " << x << " sudah ada di " << nama
                << " (himpunan tidak boleh ada Anggota ganda). Masukkan Anggota lain.\n";
        } else {
            H.push_back(x);
        }
    }
    sort(H.begin(), H.end());
    return H;
}

// ---------------------------------------------------------------------
//  OPERASI INTI (dipakai juga untuk pengecekan)
// ---------------------------------------------------------------------
Himpunan irisan(const Himpunan &A, const Himpunan &B)
{
    Himpunan H;
    for (size_t i = 0; i < A.size(); i++)
        if (ada(B, A[i])) H.push_back(A[i]);
    return H;
}

Himpunan gabungan(const Himpunan &A, const Himpunan &B)
{
    Himpunan H = A;
    for (size_t i = 0; i < B.size(); i++)
        if (!ada(A, B[i])) H.push_back(B[i]);
    sort(H.begin(), H.end());
    return H;
}

Himpunan selisih(const Himpunan &A, const Himpunan &B)
{
    Himpunan H;
    for (size_t i = 0; i < A.size(); i++)
        if (!ada(B, A[i])) H.push_back(A[i]);
    return H;
}

Himpunan bedaSetangkup(const Himpunan &A, const Himpunan &B)
{
    return gabungan(selisih(A, B), selisih(B, A));
}

// ---------------------------------------------------------------------
//  PROSES + LANGKAH-LANGKAH (step-by-step)
// ---------------------------------------------------------------------
void prosesIrisan(const Himpunan &A, const Himpunan &B)
{
    judul("1. IRISAN (A irisan B)");
    cout << "Rumus : A irisan B = {x | x anggota A dan x anggota B}\n\n";
    cout << "Penyelesaian:\n";
    cout << "Langkah 1: Periksa setiap Anggota A, apakah juga ada di B.\n";
    if (A.empty()) cout << "   (A kosong, tidak ada Anggota yang diperiksa)\n";
    Himpunan hasil;
    for (size_t i = 0; i < A.size(); i++) {
        bool di = ada(B, A[i]);
        cout << "   " << A[i] << " ada di B? "
            << (di ? "Ya    -> masuk irisan" : "Tidak -> tidak masuk irisan") << "\n";
        if (di) hasil.push_back(A[i]);
    }
    cout << "Langkah 2: Kumpulkan Anggota yang lolos pemeriksaan.\n";
    cout << "   A irisan B = " << tulis(hasil) << "\n";
    cout << "Langkah 3: Hitung kardinalitas.\n";
    cout << "   |A irisan B| = " << hasil.size() << "\n";

    cout << "\nJawaban akhir : A irisan B = " << tulis(hasil) << "\n";

    Himpunan cek = irisan(B, A);
    cout << "Pengecekan    : B irisan A = " << tulis(cek) << " -> "
        << (cek == hasil ? "sama (sifat komutatif terpenuhi)" : "TIDAK sama") << "\n";
}

void prosesGabungan(const Himpunan &A, const Himpunan &B)
{
    judul("2. GABUNGAN (A gabungan B)");
    cout << "Rumus : A gabungan B = {x | x anggota A atau x anggota B}\n\n";
    cout << "Penyelesaian:\n";
    cout << "Langkah 1: Tulis semua Anggota A.\n";
    cout << "   Hasil sementara = " << tulis(A) << "\n";
    cout << "Langkah 2: Periksa setiap Anggota B, tambahkan jika belum ada di A.\n";
    if (B.empty()) cout << "   (B kosong, tidak ada Anggota yang diperiksa)\n";
    for (size_t i = 0; i < B.size(); i++) {
        bool di = ada(A, B[i]);
        cout << "   " << B[i] << " sudah ada di A? "
            << (di ? "Ya    -> tidak ditambah (agar tidak ganda)" : "Tidak -> ditambahkan") << "\n";
    }
    Himpunan hasil = gabungan(A, B);
    cout << "Langkah 3: Urutkan Anggota hasil.\n";
    cout << "   A gabungan B = " << tulis(hasil) << "\n";
    cout << "Langkah 4: Hitung kardinalitas.\n";
    cout << "   |A gabungan B| = " << hasil.size() << "\n";

    cout << "\nJawaban akhir : A gabungan B = " << tulis(hasil) << "\n";

    int irs = (int)irisan(A, B).size();
    int rumus = (int)A.size() + (int)B.size() - irs;
    cout << "Pengecekan    : |A| + |B| - |A irisan B| = " << A.size() << " + " << B.size()
        << " - " << irs << " = " << rumus << " -> "
        << (rumus == (int)hasil.size() ? "sama dengan hasil" : "TIDAK sama") << "\n";
}

void prosesSelisih(const Himpunan &X, const Himpunan &Y, const string &nx, const string &ny)
{
    string op = nx + " - " + ny;
    cout << "\n--- " << op << " ---\n";
    cout << "Rumus : " << op << " = {x | x anggota " << nx << " dan x bukan anggota " << ny << "}\n";
    cout << "Langkah 1: Periksa setiap Anggota " << nx << ", apakah TIDAK ada di " << ny << ".\n";
    if (X.empty()) cout << "   (" << nx << " kosong, tidak ada Anggota yang diperiksa)\n";
    Himpunan hasil;
    for (size_t i = 0; i < X.size(); i++) {
        bool di = ada(Y, X[i]);
        cout << "   " << X[i] << " ada di " << ny << "? "
            << (di ? "Ya    -> dibuang" : "Tidak -> masuk hasil") << "\n";
        if (!di) hasil.push_back(X[i]);
    }
    cout << "Langkah 2: Kumpulkan Anggota yang masuk hasil.\n";
    cout << "   " << op << " = " << tulis(hasil) << "\n";
    cout << "Langkah 3: Hitung kardinalitas.\n";
    cout << "   |" << op << "| = " << hasil.size() << "\n";
    int irs = (int)irisan(X, Y).size();
    int rumus = (int)X.size() - irs;
    cout << "Jawaban akhir : " << op << " = " << tulis(hasil) << "\n";
    cout << "Pengecekan    : |" << nx << "| - |" << nx << " irisan " << ny << "| = "
        << X.size() << " - " << irs << " = " << rumus << " -> "
        << (rumus == (int)hasil.size() ? "sama dengan hasil" : "TIDAK sama") << "\n";
}

void prosesSelisihLengkap(const Himpunan &A, const Himpunan &B)
{
    judul("3. SELISIH (A - B dan B - A)");
    prosesSelisih(A, B, "A", "B");
    prosesSelisih(B, A, "B", "A");
    cout << "\nCatatan: A - B tidak selalu sama dengan B - A.\n";
}

void prosesBedaSetangkup(const Himpunan &A, const Himpunan &B)
{
    judul("4. BEDA SETANGKUP (A beda-setangkup B)");
    cout << "Rumus : A (+) B = (A - B) gabungan (B - A)\n\n";
    cout << "Penyelesaian:\n";
    Himpunan ab = selisih(A, B);
    Himpunan ba = selisih(B, A);
    cout << "Langkah 1: Cari A - B (Anggota A yang tidak ada di B).\n";
    cout << "   A - B = " << tulis(ab) << "\n";
    cout << "Langkah 2: Cari B - A (Anggota B yang tidak ada di A).\n";
    cout << "   B - A = " << tulis(ba) << "\n";
    Himpunan hasil = gabungan(ab, ba);
    cout << "Langkah 3: Gabungkan kedua hasil.\n";
    cout << "   (A - B) gabungan (B - A) = " << tulis(hasil) << "\n";
    cout << "Langkah 4: Hitung kardinalitas.\n";
    cout << "   |A (+) B| = " << hasil.size() << "\n";

    cout << "\nJawaban akhir : A (+) B = " << tulis(hasil) << "\n";

    Himpunan gab = gabungan(A, B);
    Himpunan irs = irisan(A, B);
    Himpunan cek = selisih(gab, irs);
    cout << "Pengecekan    : Dengan rumus lain, A (+) B = (A gabungan B) - (A irisan B)\n";
    cout << "                (A gabungan B) = " << tulis(gab) << "\n";
    cout << "                (A irisan B)   = " << tulis(irs) << "\n";
    cout << "                Selisihnya     = " << tulis(cek) << " -> "
        << (cek == hasil ? "sama dengan hasil" : "TIDAK sama") << "\n";
}

// ---------------------------------------------------------------------
//  MENU UTAMA
// ---------------------------------------------------------------------
void tampilData(const Himpunan &A, const Himpunan &B)
{
    cout << "\nData saat ini:\n";
    cout << "  A = " << tulis(A) << "   (|A| = " << A.size() << ")\n";
    cout << "  B = " << tulis(B) << "   (|B| = " << B.size() << ")\n";
}

void inputData(Himpunan &A, Himpunan &B)
{
    judul("INPUT HIMPUNAN");
    cout << "Masukkan bilangan bulat. Anggota yang sama tidak diizinkan.\n\n";
    A = bacaHimpunan("A");
    cout << "\n";
    B = bacaHimpunan("B");
    tampilData(A, B);
}

void tampilMenu()
{
    cout << "\n==================================================\n";
    cout << "     XGhidra - Simulasi Operasi Himpunan\n";
    cout << "==================================================\n";
    cout << " [1] Input himpunan (A dan B)\n";
    cout << " [2] Irisan\n";
    cout << " [3] Gabungan\n";
    cout << " [4] Selisih (A - B dan B - A)\n";
    cout << " [5] Beda Setangkup\n";
    cout << " [6] Jalankan semua operasi\n";
    cout << " [0] Keluar\n";
    cout << "--------------------------------------------------\n";
}

int main()
{
    Himpunan A, B;
    bool siap = false;
    int pilihan;

    do {
        tampilMenu();
        if (siap) tampilData(A, B);
        pilihan = bacaAngka("\nPilih menu (0-6): ");

        if (pilihan >= 2 && pilihan <= 6 && !siap) {
            cout << "\nHimpunan belum diisi. Pilih menu 1 dulu.\n";
            continue;
        }

        switch (pilihan) {
            case 1:
                inputData(A, B);
                siap = true;
                break;
            case 2: prosesIrisan(A, B); break;
            case 3: prosesGabungan(A, B); break;
            case 4: prosesSelisihLengkap(A, B); break;
            case 5: prosesBedaSetangkup(A, B); break;
            case 6:
                prosesIrisan(A, B);
                prosesGabungan(A, B);
                prosesSelisihLengkap(A, B);
                prosesBedaSetangkup(A, B);
                break;
            case 0:
                cout << "\nTerima kasih telah memakai XGhidra.\n";
                break;
            default:
                cout << "\nPilihan tidak ada. Pilih angka 0 sampai 6.\n";
        }
    } while (pilihan != 0);

    return 0;
}
