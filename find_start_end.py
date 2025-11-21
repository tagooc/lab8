import os
import gspread
import json
from google.oauth2.service_account import Credentials
from gspread_formatting import *
import time

# 📊 Подключаемся к Google Таблице
try:
    # Используем правильные scope
    scope = ['https://spreadsheets.google.com/feeds', 'https://www.googleapis.com/auth/drive']
    creds = Credentials.from_service_account_file('credentials.json', scopes=scope)
    client = gspread.authorize(creds)
    # Открываем таблицу и выбираем первый лист
    sheet = client.open("maze").sheet1
    print("✅ Google Sheets подключен")
except Exception as e:
    print(f"❌ Ошибка Google Sheets: {e}")
    exit()

def find_colored_cells():
    """
    Ищет зеленую (старт) и красную (финиш) клетки в таблице по цвету заливки
    Возвращает словарь с координатами
    """
    try:
        # Получаем все данные из таблицы, чтобы определить ее размеры
        all_data = sheet.get_all_values()
        num_rows = len(all_data)
        num_cols = len(all_data[0]) if all_data else 0
        print(f"Таблица имеет размеры: {num_rows}x{num_cols}")

        if num_rows == 0 or num_cols == 0:
            print("Таблица пуста.")
            return {}

        result = {}

        # Проходим по всем ячейкам и проверяем их форматирование
        for row in range(1, num_rows + 1):
            for col in range(1, num_cols + 1):
                time.sleep(0.1)
                try:
                    # Получаем формат конкретной ячейки
                    cell_format = get_user_entered_format(sheet, f'{gspread.utils.rowcol_to_a1(row, col)}')
                    
                    if True:
                        color = cell_format.backgroundColor
                        print(color, col, row)
                        # Проверяем на зеленый цвет (Старт) - высокий зеленый, низкие красный и синий
                        if color.green == 1 and not color.blue and not color.red :
                            # Координаты возвращаем в формате (col, row) (X, Y)
                            # Вычитаем 1, потому что индексы в Python начинаются с 0
                            result["start"] = (col - 1, row - 1)
                            print(f"🚀 Найден старт (зеленый) в ({col - 1}, {row - 1})")

                        # Проверяем на красный цвет (Финиш) - высокий красный, низкие зеленый и синий
                        if color.red == 1 and not color.blue and not color.green:
                            result["end"] = (col - 1, row - 1)
                            print(f"🎯 Найден финиш (красный) в ({col - 1}, {row - 1})")
                            
                except Exception as cell_error:
                    # Пропускаем ячейки с ошибками форматирования
                    print(cell_error)
                    continue

        if "start" not in result or "end" not in result:
            print("⚠️ Не удалось найти обе ячейки (старт/финиш) по цвету.")
        
        return result
        
    except Exception as e:
        print(f"❌ Ошибка при поиске цветных ячеек: {e}")
        return {}


def save_to_json(data, filename="breaks.json"):
    """Сохраняет координаты в JSON файл"""
    try:
        # Преобразуем tuple в list для JSON
        json_data = {}
        if "start" in data:
            json_data["start"] = str(tuple(data["start"]))
        if "end" in data:
            json_data["end"] = str(tuple(data["end"]))
            
        with open(filename, 'w', encoding='utf-8') as f:
            json.dump(json_data, f, indent=2, ensure_ascii=False)
        print(f"✅ Координаты сохранены в {filename}")
        print("📊 Результат:")
        print(json.dumps(json_data, indent=2, ensure_ascii=False))
    except Exception as e:
        print(f"❌ Ошибка при сохранении в JSON: {e}")

# Основная логика
def main():
    print("🔍 Поиск координат старта и финиша...")
    
    coordinates = {}
    # 1. Сначала пробуем найти по цвету
    print("\n🎨 Метод 1: Поиск по цвету...")
    coordinates = find_colored_cells()

    
    # Сохраняем результат
    save_to_json(coordinates)

# Запуск
if __name__ == "__main__":
    main()