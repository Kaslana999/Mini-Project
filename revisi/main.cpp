// =====================================================================
//  XGhidra - Aplikasi Simulasi Operasi Himpunan (Matematika Diskrit)
//
//  Operasi yang diselesaikan:
//    1. Irisan
//    2. Gabungan
//    3. Selisih
//    4. Komplemen
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

bool subhimpunan(const Himpunan &X, const Himpunan &S)
{
    for (size_t i = 0; i < X.size(); i++)
        if (!ada(S, X[i])) return false;
    return true;
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

void prosesKomplemen(const Himpunan &X, const string &nx, const Himpunan &S)
{
    string op = nx + "'";
    cout << "\n--- Komplemen " << nx << " (" << op << ") ---\n";
    cout << "Rumus : " << op << " = S - " << nx << " = {x | x anggota S dan x bukan anggota " << nx << "}\n";
    cout << "Langkah 1: Periksa setiap Anggota semesta S, apakah TIDAK ada di " << nx << ".\n";
    if (S.empty()) cout << "   (S kosong, tidak ada Anggota yang diperiksa)\n";
    Himpunan hasil;
    for (size_t i = 0; i < S.size(); i++) {
        bool di = ada(X, S[i]);
        cout << "   " << S[i] << " ada di " << nx << "? "
            << (di ? "Ya    -> dibuang" : "Tidak -> masuk komplemen") << "\n";
        if (!di) hasil.push_back(S[i]);
    }
    cout << "Langkah 2: Kumpulkan Anggota yang masuk komplemen.\n";
    cout << "   " << op << " = " << tulis(hasil) << "\n";
    cout << "Langkah 3: Hitung kardinalitas.\n";
    cout << "   |" << op << "| = " << hasil.size() << "\n";
    cout << "Jawaban akhir : " << op << " = " << tulis(hasil) << "\n";

    int rumus = (int)S.size() - (int)X.size();
    Himpunan gab = gabungan(X, hasil);
    cout << "Pengecekan    : (1) |S| - |" << nx << "| = " << S.size() << " - " << X.size()
        << " = " << rumus << " -> " << (rumus == (int)hasil.size() ? "sama" : "TIDAK sama") << "\n";
    cout << "                (2) " << nx << " gabungan " << op << " = " << tulis(gab)
        << " -> " << (gab == S ? "sama dengan S" : "TIDAK sama dengan S") << "\n";
}

void prosesKomplemenLengkap(const Himpunan &A, const Himpunan &B, const Himpunan &S)
{
    judul("4. KOMPLEMEN (A' dan B')");
    cout << "Semesta S = " << tulis(S) << "\n";
    prosesKomplemen(A, "A", S);
    prosesKomplemen(B, "B", S);
}

// ---------------------------------------------------------------------
//  MENU UTAMA
// ---------------------------------------------------------------------
void tampilData(const Himpunan &A, const Himpunan &B, const Himpunan &S)
{
    cout << "\nData saat ini:\n";
    cout << "  A = " << tulis(A) << "   (|A| = " << A.size() << ")\n";
    cout << "  B = " << tulis(B) << "   (|B| = " << B.size() << ")\n";
    cout << "  S = " << tulis(S) << "   (|S| = " << S.size() << ")\n";
}

void inputData(Himpunan &A, Himpunan &B, Himpunan &S)
{
    judul("INPUT HIMPUNAN");
    cout << "Masukkan bilangan bulat. Anggota yang sama tidak diizinkan.\n\n";
    A = bacaHimpunan("A");
    cout << "\n";
    B = bacaHimpunan("B");
    cout << "\nSemesta S dipakai untuk komplemen dan harus memuat semua Anggota A dan B.\n";
    while (true) {
        S = bacaHimpunan("S");
        if (subhimpunan(A, S) && subhimpunan(B, S)) break;
        Himpunan gab = gabungan(A, B);
        Himpunan kurang = selisih(gab, S);
        cout << "S belum memuat Anggota: " << tulis(kurang) << "\n";
        cout << "Silakan masukkan ulang S.\n\n";
    }
    tampilData(A, B, S);
}

void tampilMenu()
{
    cout << "\n==================================================\n";
    cout << "     XGhidra - Simulasi Operasi Himpunan\n";
    cout << "==================================================\n";
    cout << " [1] Input himpunan (A, B, dan semesta S)\n";
    cout << " [2] Irisan\n";
    cout << " [3] Gabungan\n";
    cout << " [4] Selisih (A - B dan B - A)\n";
    cout << " [5] Komplemen (A' dan B')\n";
    cout << " [6] Jalankan semua operasi\n";
    cout << " [0] Keluar\n";
    cout << "--------------------------------------------------\n";
}

int main()
{
    Himpunan A, B, S;
    bool siap = false;
    int pilihan;

    do {
        tampilMenu();
        if (siap) tampilData(A, B, S);
        pilihan = bacaAngka("\nPilih menu (0-6): ");

        if (pilihan >= 2 && pilihan <= 6 && !siap) {
            cout << "\nHimpunan belum diisi. Pilih menu 1 dulu.\n";
            continue;
        }

        switch (pilihan) {
            case 1:
                inputData(A, B, S);
                siap = true;
                break;
            case 2: prosesIrisan(A, B); break;
            case 3: prosesGabungan(A, B); break;
            case 4: prosesSelisihLengkap(A, B); break;
            case 5: prosesKomplemenLengkap(A, B, S); break;
            case 6:
                prosesIrisan(A, B);
                prosesGabungan(A, B);
                prosesSelisihLengkap(A, B);
                prosesKomplemenLengkap(A, B, S);
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
