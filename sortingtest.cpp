#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <iomanip>

// ==========================================================
// 1. CÀI ĐẶT CÁC THUẬT TOÁN SẮP XẾP (Ý 2)
// ==========================================================

// --- 1.1. QUICKSORT (Tối ưu Hoare + Mid Pivot + Khử đệ quy đuôi) ---
void quickSort(std::vector<double>& arr, int low, int high) {
    while (low < high) {
        double pivot = arr[low + (high - low) / 2];
        int i = low;
        int j = high;

        // Phân hoạch kiểu Hoare
        while (i <= j) {
            while (arr[i] < pivot) i++;
            while (arr[j] > pivot) j--;
            if (i <= j) {
                std::swap(arr[i], arr[j]);
                i++;
                j--;
            }
        }

        // Đệ quy trên nửa ngắn hơn để khống chế độ sâu Stack <= O(log N)
        if (j - low < high - i) {
            if (low < j) quickSort(arr, low, j);
            low = i;
        } else {
            if (i < high) quickSort(arr, i, high);
            high = j;
        }
    }
}

// --- 1.2. HEAPSORT (Dùng Heapify vòng lặp tối ưu) ---
void heapify(std::vector<double>& arr, int n, int i) {
    while (true) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && arr[left] > arr[largest])
            largest = left;
        if (right < n && arr[right] > arr[largest])
            largest = right;

        if (largest == i) break;

        std::swap(arr[i], arr[largest]);
        i = largest;
    }
}

void heapSort(std::vector<double>& arr) {
    int n = arr.size();
    // Tạo Max-Heap ban đầu
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }
    // Trích xuất từng phần tử ra khỏi Heap
    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// --- 1.3. MERGESORT (Dùng chung 1 mảng tạm duy nhất) ---
void merge(std::vector<double>& arr, std::vector<double>& temp, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (int idx = left; idx <= right; ++idx) {
        arr[idx] = temp[idx];
    }
}

void mergeSortInternal(std::vector<double>& arr, std::vector<double>& temp, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortInternal(arr, temp, left, mid);
    mergeSortInternal(arr, temp, mid + 1, right);
    merge(arr, temp, left, mid, right);
}

void mergeSort(std::vector<double>& arr) {
    std::vector<double> temp(arr.size());
    mergeSortInternal(arr, temp, 0, (int)arr.size() - 1);
}

// ==========================================================
// 2. HÀM ĐỌC DỮ LIỆU TỪ FILE
// ==========================================================
std::vector<double> readData(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "Khong the mo file: " << filename << "\n";
        return {};
    }
    std::ios_base::sync_with_stdio(false);
    fin.tie(NULL);

    int n;
    fin >> n;
    std::vector<double> arr(n);
    for (int i = 0; i < n; ++i) {
        fin >> arr[i];
    }
    return arr;
}

// ==========================================================
// 3. THỬ NGHIỆM VÀ ĐO THỜI GIAN (Ý 3)
// ==========================================================
struct Result {
    double timeStdSort;
    double timeQuickSort;
    double timeHeapSort;
    double timeMergeSort;
};

int main() {
    std::vector<Result> results;
    std::cout << "BAT DAU THU NGHIEM TREN 10 BO DU LIEU (1.000.000 PHAN TU)...\n\n";

    for (int i = 1; i <= 10; ++i) {
        std::string filename = "input_" + std::to_string(i) + ".txt";
        std::cout << "-> Dang doc file " << filename << "... ";
        
        std::vector<double> originalData = readData(filename);
        if (originalData.empty()) {
            std::cout << "[BO QUA VI KHONG TIM THAY FILE]\n";
            continue;
        }
        std::cout << "Xong. Dang chay test...\n";

        Result r;

        // 1. std::sort C++
        {
            std::vector<double> a = originalData;
            auto t1 = std::chrono::high_resolution_clock::now();
            std::sort(a.begin(), a.end());
            auto t2 = std::chrono::high_resolution_clock::now();
            r.timeStdSort = std::chrono::duration<double, std::milli>(t2 - t1).count();
        }

        // 2. QuickSort
        {
            std::vector<double> a = originalData;
            auto t1 = std::chrono::high_resolution_clock::now();
            quickSort(a, 0, (int)a.size() - 1);
            auto t2 = std::chrono::high_resolution_clock::now();
            r.timeQuickSort = std::chrono::duration<double, std::milli>(t2 - t1).count();
        }

        // 3. HeapSort
        {
            std::vector<double> a = originalData;
            auto t1 = std::chrono::high_resolution_clock::now();
            heapSort(a);
            auto t2 = std::chrono::high_resolution_clock::now();
            r.timeHeapSort = std::chrono::duration<double, std::milli>(t2 - t1).count();
        }

        // 4. MergeSort
        {
            std::vector<double> a = originalData;
            auto t1 = std::chrono::high_resolution_clock::now();
            mergeSort(a);
            auto t2 = std::chrono::high_resolution_clock::now();
            r.timeMergeSort = std::chrono::duration<double, std::milli>(t2 - t1).count();
        }

        results.push_back(r);
    }

    // In bảng tổng hợp kết quả (Dạng Markdown) phục vụ cho Ý 4
    std::cout << "\n============================== KET QUA THU NGHIEM (ms) ==============================\n";
    std::cout << std::left 
              << std::setw(15) << "| Bo du lieu" 
              << std::setw(18) << "| std::sort (ms)" 
              << std::setw(18) << "| QuickSort (ms)" 
              << std::setw(18) << "| HeapSort (ms)" 
              << std::setw(18) << "| MergeSort (ms)" 
              << "|\n";
    std::cout << "|--------------|-----------------|-----------------|-----------------|-----------------|\n";

    for (size_t i = 0; i < results.size(); ++i) {
        std::string label = "Day " + std::to_string(i + 1);
        if (i == 0) label += " (Tang)";
        else if (i == 1) label += " (Giam)";
        else label += " (Ran)";

        std::cout << "| " << std::left << std::setw(13) << label
                  << "| " << std::fixed << std::setprecision(2) << std::setw(16) << results[i].timeStdSort
                  << "| " << std::fixed << std::setprecision(2) << std::setw(16) << results[i].timeQuickSort
                  << "| " << std::fixed << std::setprecision(2) << std::setw(16) << results[i].timeHeapSort
                  << "| " << std::fixed << std::setprecision(2) << std::setw(16) << results[i].timeMergeSort
                  << "|\n";
    }
    std::cout << "====================================================================================\n";

    return 0;
}