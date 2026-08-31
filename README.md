# 🔍 C Memory Leak Detector 

Dinamik bellek yönetimi (heap memory) kullanan C programlarında ayrılan fakat serbest bırakılmayan bellek bloklarını dosya adı ve satır numarasıyla birlikte tespit eden hafif bir hata ayıklama aracı.

---

## 📌 Proje Hakkında

C dilinde dinamik bellek yönetimi geliştiricinin sorumluluğundadır. `malloc` ile ayrılan bir alanın `free` ile serbest bırakılması unutulduğunda **bellek sızıntısı (memory leak)** meydana gelir.

Bu proje;
* C ön işlemci makrolarını (`#define`, `__FILE__`, `__LINE__`) kullanarak standart `malloc` ve `free` çağrılarını kancalar (intercept eder).
* Ayrılan her bloğu bir **Bağlı Liste (Linked List)** üzerinde meta verileriyle saklar.
* Serbest bırakılan blokları listeden düşer.
* Program sonunda listede kalan (serbest bırakılmamış) tüm blokları raporlar.

---

```
Kodu Çalıştırma:
```powershell
gcc -Iinclude src/leak-detector.c src/main.c -o leak-detector.exe
.\leak-detector.exe
