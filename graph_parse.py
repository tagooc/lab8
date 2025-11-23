import os
import gspread
from gspread_formatting import *
import json
from google.oauth2.service_account import Credentials
import time

from config_loader import Config
cfg = Config()

# 📊 Подключаемся к Google Таблице
try:
    scope = ['https://spreadsheets.google.com/feeds', 'https://www.googleapis.com/auth/drive']
    creds = Credentials.from_service_account_file('credentials.json', scopes=scope)
    client = gspread.authorize(creds)
    sheet = client.open(cfg.sheet_name).sheet1
    print("✅ Google Sheets подключен")
except Exception as e:
    print(f"❌ Ошибка Google Sheets: {e}")
    exit()

def parse_white_cells_to_json(sheet):
    """
    Парсит белые клетки как вершины графа
    Координаты вершины = (row, column) ячейки
    Белый цвет: RGB(1, 1, 1)
    """
    graph = {}
    
    try:
        # Получаем все ячейки с форматированием
        all_data = sheet.get_all_values()
        num_rows = len(all_data)
        num_cols = len(all_data[0]) if all_data else 0
        print(f"Таблица имеет размеры: {num_rows}x{num_cols}")

        if num_rows == 0 or num_cols == 0:
            print("Таблица пуста.")
            return {}

        white_cells = []
        start_end = {}
        # Проходим по всем ячейкам и проверяем их форматирование
        for row in range(1, num_rows + 1):
            for col in range(1, num_cols + 1):
                time.sleep(cfg.sleep_time)
                try:
                    # Получаем формат конкретной ячейки
                    cell_format = get_user_entered_format(sheet, f'{gspread.utils.rowcol_to_a1(row, col)}')

                    if True:
                        color = cell_format.backgroundColor
                        print(color, col, row)
                        if color.green == 1 and not color.blue and not color.red :
                            start_end["start"] = (col - 1, row - 1)
                            print(f"🚀 Найден старт (зеленый) в ({col - 1}, {row - 1})")

                        # Проверяем на красный цвет (Финиш) - высокий красный, низкие зеленый и синий
                        elif color.red == 1 and not color.blue and not color.green:
                            start_end["end"] = (col - 1, row - 1)
                            print(f"🎯 Найден финиш (красный) в ({col - 1}, {row - 1})")
                            
                            
                        if (color.green == 1 and color.blue == 1 and color.red == 1)  or (color.green == 1 and not color.blue and not color.red) or (color.red == 1 and not color.blue and not color.green):
                            white_cells.append((col - 1, row - 1))

                            
                except Exception as cell_error:
                    # Пропускаем ячейки с ошибками форматирования
                    print(cell_error)
                    continue
        
        if "start" not in start_end or "end" not in start_end:
            print("⚠️ Не удалось найти обе ячейки (старт/финиш) по цвету.")
            
        # Теперь для каждой белой ячейки ищем соседей
        for row, col in white_cells:
            neighbors = []
            vertex = (row, col)
            
            # Проверяем 4-связность (верх, низ, лево, право)
            directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]  # up, down, left, right
            
            for dr, dc in directions:
                neighbor_row, neighbor_col = row + dr, col + dc
                neighbor_coord = (neighbor_row, neighbor_col)
                
                if neighbor_coord in white_cells:
                    neighbors.append(neighbor_coord)
            
            # Добавляем вершину в граф
            graph[str(vertex)] = [str(n) for n in neighbors]
            print(f"✅ Вершина {vertex} -> соседи: {neighbors}")
    
    except Exception as e:
        print(f"❌ Ошибка при парсинге: {e}")
        return
    
    # Сохраняем в JSON
    try:
        with open(cfg.file_graph, 'w', encoding='utf-8') as f_graph:
            json.dump(graph, f_graph, indent=4, ensure_ascii=False)
        print(f"✅ Граф сохранен в {cfg.file_graph}")
        print(f"📊 Всего вершин: {len(graph)}")
    except Exception as e:
        print(f"❌ Ошибка при сохранении JSON graph: {e}")
    
    try:
        # Преобразуем tuple в list для JSON
        json_data = {}
        if "start" in start_end:
            json_data["start"] = str(tuple(start_end["start"]))
        if "end" in start_end:
            json_data["end"] = str(tuple(start_end["end"]))
            
        with open(cfg.file_breaks, 'w', encoding='utf-8') as f:
            json.dump(json_data, f, indent=2, ensure_ascii=False)
        print(f"✅ Координаты сохранены в {cfg.file_graph}")
        print("📊 Результат:")
        print(json.dumps(json_data, indent=2, ensure_ascii=False))
    except Exception as e:
        print(f"❌ Ошибка при сохранении в JSON: {e}")

# Запускаем парсинг
if __name__ == "__main__":
    parse_white_cells_to_json(sheet)