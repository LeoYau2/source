#include <iostream>
#include <math.h>
#include <windows.h>
#include <iomanip>

using namespace std;

// 將AWG編號轉換為直徑(米)
double awg_to_diameter(int awg) {
  double diameter = 0.005 * pow(92, ((36 - awg) / 39.0));
  return diameter / 39.37;
}

// 計算趨膚效應下的電阻率
double skin_effect(double resistivity, double skin_depth, double depth) {
  return resistivity * (1 + pow(depth / skin_depth, 2));
}

// 計算分支後的直徑
double calculate_branch_diameter(double original_diameter, int branches) {
  return original_diameter * sqrt(1.0 / branches);
}

int main() {
  system("chcp 65001");
  // 選擇材質
  string input;
  cout << "請選擇材質('c' 為銅,'s' 為銀):";
  cin >> input;

  double resistivity;
  double permeability;

  if (input == "c") {
    resistivity = 1.7e-8;
    permeability = 1.256629e-6;
  } else if (input == "s") {
    resistivity = 1.59e-8;
    permeability = 1.256637e-6;
  } else {
    cout << "無效輸入" << endl;
    return 0;
  }
  cout << "電阻率:" << resistivity << " Ωm, 磁導率:" << permeability << " H/m" << endl;

  // 選擇輸入方式
  string type;
  cout << "請選擇輸入方式('a' 為AWG, 'd' 為直徑):";
  cin >> type;

  double original_diameter;

  if (type == "a") {
    int awg;
    cout << "請輸入AWG編號:";
    cin >> awg;
    original_diameter = awg_to_diameter(awg);
    cout << "AWG" << awg << "對應的直徑為" << original_diameter << "米" << endl;
  } else if (type == "d") {
    cout << "請輸入原始直徑(米):";
    cin >> original_diameter;
  } else {
    cout << "無效輸入" << endl;
    return 0;
  }

  // 輸入分支數、長度
  int branches;
  double length;
  cout << "請輸入分支數:";
  cin >> branches;
  cout << "分支數" << branches << "，分支直徑" << original_diameter / sqrt(branches) << "米" << endl;
  cout << "請輸入線長(米):";
  cin >> length;

  // 循環詢問頻率
  while (true) {

    double frequency;
    cout << "請輸入頻率(Hz):";
    cin >> frequency;

    if (frequency == 0) break;

    // 計算直流電阻
    double dc_resistance = resistivity * length / (M_PI * pow(original_diameter/2, 2));

    // 計算分支半徑
    double radius = original_diameter / 2 / sqrt(branches);

    // 計算趨膚深度
    double skin_depth = sqrt(resistivity / (M_PI * frequency * permeability));

    // 分環計算AC電阻
    double resistence = 0;
    for (int i = 0; i < 4096; i++) {
      double inner_radius = radius * i / 4096;
      double outer_radius = radius * (i+1) / 4096;
      double area = M_PI * (pow(outer_radius, 2) - pow(inner_radius, 2));
      resistence += 1 / (skin_effect(resistivity, skin_depth, radius - radius*i/4096) * length / area);
    }
    resistence = 1 / resistence / branches;

    // 輸出結果
    double ratio = resistence / dc_resistance * 100;
    cout << "頻率:" << frequency << " Hz" << endl;
    cout << "直流電阻:" << dc_resistance << " Ω" << endl;
    cout << "趨膚效應電阻:" << resistence << " Ω, 倍率:" << fixed << setprecision(4) << ratio << " %" << endl;
  }

  return 0;
}
