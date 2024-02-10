#pragma once
#include <fstream>
#include "method_forward_declarations.h"
namespace Level1
{
	namespace L
	{
		String ToString(const Line& l)
		{
			String s(15);
			UINT16 u16;
			u16 = l.start.x;
			s[0] = u16;
			s[1] = u16 >> 8;

			u16 = l.start.y;
			s[2] = u16;
			s[3] = u16 >> 8;

			u16 = l.end.x;
			s[4] = u16;
			s[5] = u16 >> 8;

			u16 = l.end.y;
			s[6] = u16;
			s[7] = u16 >> 8;

			s[8] = l.width.u;
			s[9] = l.width.u >> 8;

			s[10] = l.width.b;

			s[11] = l.color.GetA();
			s[12] = l.color.GetR();
			s[13] = l.color.GetG();
			s[14] = l.color.GetB();
			return s;
		}
		const char PolylineEnd1[15] = { (char)0, (char)1, (char)2,(char)3, (char)4, (char)5, (char)6, (char)7, (char)8,
			(char)9, (char)10, (char)0, (char)0, (char)13,(char)14 };
		String PolylineEnd(15, PolylineEnd1);
		const char ShapeEnd1[15] = { (char)0, (char)1, (char)2,(char)3, (char)4, (char)5, (char)6, (char)7, (char)8,
			(char)9, (char)10, (char)0, (char)1, (char)13,(char)14 };
		String ShapeEnd(15, ShapeEnd1);
		const char FrameEnd1[15] = { (char)0, (char)1, (char)2,(char)3, (char)4, (char)5, (char)6, (char)7, (char)8,
			(char)9, (char)10, (char)0, (char)2, (char)13,(char)14 };
		String FrameEnd(15, FrameEnd1);
		Line ToLine(String s, Polyline* p)
		{
			StrokeWidth sw;
			sw.u = s[8] | (s[9] << 8);
			sw.b = s[10];
			return Line(p, Point((UINT32)s[0] | ((UINT32)s[1] << 8), (UINT32)s[2] | ((UINT32)s[3] << 8)),
				Point((UINT32)s[4] | ((UINT32)s[5] << 8), (UINT32)s[6] | ((UINT32)s[7] << 8)),
				Gdiplus::Color::MakeARGB(s[11], s[12], s[13], s[14]),
				sw);
		}
		String ToString(const Polyline& p)
		{
			String s(p.size() * 15);
			size_t i = 0;
			for (const Line& l : p)
			{
				String s1 = ToString(l);
				for (char c : s1)
				{
					s[i] = c;
					i++;
				}
			}
			return s;
		}
		void ToPolyline(String s, Shape* sh)
		{
			sh->push_front(Polyline(sh));
			Polyline& p = *sh->begin();
			String s1(15);
			for (size_t i = 0; i < s.size(); i += 15)
			{
				for (size_t j = 0; j < 15; j++)
					s1[j] = s[i + j];
				p.push_front(ToLine(s1, &p));
			}
		}
		String ToString(const Shape& sh)
		{
			size_t size = 0;
			for (const Polyline& p : sh)
				size += p.size() * 15 + 15;
			String s(size);
			size_t i = 0;
			for (const Polyline& p : sh)
			{
				String s1 = ToString(p);
				for (char c : s1)
				{
					s[i] = c;
					i++;
				}
				for (char c : PolylineEnd)
				{
					s[i] = c;
					i++;
				}
			}
			return s;
		}
		void ToShape(String s, TouchMap* t)
		{
			t->push_front(Shape());
			Shape& sh = *t->begin();
			String s1(60000);
			size_t k = 0;
			for (size_t i = 0; i < s.size(); i += 15)
			{
				if ((s[i + 11] == 0) and (s[i + 12] == 0))
				{
					s1.n = k;
					ToPolyline(s1, &sh);
					k = 0;
				}
				else
				{
					for (size_t j = 0; j < 15; j++)
					{
						s1[k] = s[i + j];
						k++;
					}
				}
			}
		}
		String ToString(const TouchMap& t)
		{
			size_t size = 0;
			for (const Shape& sh : t)
			{
				for (const Polyline& p : sh)
					size += p.size() * 15 + 15;
				size += 15;
			}
			String s(size);
			size_t i = 0;
			for (const Shape& sh : t)
			{
				String s1 = ToString(sh);
				for (char c : s1)
				{
					s[i] = c;
					i++;
				}
				for (char c : ShapeEnd)
				{
					s[i] = c;
					i++;
				}
			}
			return s;
		}
		TouchMap ToTouchMap(String s)
		{
			TouchMap t;
			String s1(60000);
			size_t k = 0;
			for (size_t i = 0; i < s.size(); i += 15)
			{
				if ((s[i + 11] == 0) and (s[i + 12] == 1))
				{
					s1.n = k;
					ToShape(s1, &t);
					k = 0;
				}
				else
				{
					for (size_t j = 0; j < 15; j++)
					{
						s1[k] = s[i + j];
						k++;
					}
				}
			}
			return t;
		}
		String ToString(const Animation* const a)
		{
			size_t size = 4;
			for (const Frame& f : *a)
			{
				for (const Shape& sh : f.tm)
				{
					for (const Polyline& p : sh)
						size += p.size() * 15 + 15;
					size += 15;
				}
				size += 15;
			}
			String s(size);
			s[0] = a->w;
			s[1] = a->w >> 8;
			s[2] = a->h;
			s[3] = a->h >> 8;
			size_t i = 4;
			for (const Frame& f : *a)
			{
				String s1 = ToString(f.tm);
				for (char c : s1)
				{
					s[i] = c;
					i++;
				}
				for (char c : FrameEnd)
				{
					s[i] = c;
					i++;
				}
			}
			return s;
		}
		Animation* ToAnimation(String s)
		{
			Animation* a = new Animation((ULONG)s[0] | ((ULONG)s[1] << 8), (ULONG)s[2] | ((ULONG)s[3] << 8));
			a->clear();
			String s1(60000);
			size_t k = 0;
			for (size_t i = 4; i < s.size(); i += 15)
			{
				if ((s[i + 11] == 0) and (s[i + 12] == 2))
				{
					s1.n = k;
					a->push_back(Frame(a->w, a->h, Gdiplus::Color::Red));
					Frame& f = *a->rbegin();
					f.tm = ToTouchMap(s1);
					f.tm.deepDraw(f.pm);
					f.pm->deepDraw(f.b);
					k = 0;
				}
				else
				{
					for (size_t j = 0; j < 15; j++)
					{
						s1[k] = s[i + j];
						k++;
					}
				}
			}
			return a;
		}
	}
	class MenuCreater
	{
	public:
		enum ButtonNumber
		{
			NEW = 20, OPEN, _2DSMAX_FILE, IMAGE,
				SAVE_ANIMATION, SAVE_ANIMATION_AS,
			UNDO, REDO, CUT, COPY, PAST, DESELECT, DEL, MID,
			BACK, FORWARD, PREVIOUS_FRAME, NEXT_FRAME, CHOOSE_FRAME, ADD_FRAME,
			DRAW,
				LOCATION, ROTATION, SCALE,
				ITEM_LOCATION, ITEM_ROTATION, ITEM_SCALE,
				CENTER_LOCATION, CENTER_ROTATION, CENTER_SCALE,
			COLOR, WIDTH,
			SHOW_AT_FIRST, STOP_SHOWING, FRAME_RATE,
		};
		Animation* animation = new Animation(800, 800);
		HWND hwnd;
		HINSTANCE hInstance;
		LRESULT command(WPARAM wParam)
		{
			switch (wParam)
			{
			case 1:
				//animation->params.color = animation->selectedPixelInfo.color;
				//DestroyWindow(animation->copyColorButton);
				//DestroyWindow(animation->copyWidthButton);
				return 0;
			case 2:
				//animation->params.width = animation->selectedPixelInfo.width;
				//DestroyWindow(animation->copyColorButton);
				//DestroyWindow(animation->copyWidthButton);
				return 0;
			case DRAW:
				animation->params.isDrawingMode = true;
				return 0;
			case LOCATION:
				animation->params.mode = EditMode::LOCATION;
				animation->params.isDrawingMode = false;
				return 0;
			case ROTATION:
				animation->params.mode = EditMode::ROTATION;
				animation->params.isDrawingMode = false;
				return 0;
			case SCALE:
				animation->params.mode = EditMode::SCALE;
				animation->params.isDrawingMode = false;
				return 0;
			case ITEM_LOCATION:
				animation->params.mode = EditMode::ITEM_LOCATION;
				animation->params.isDrawingMode = false;
				return 0;
			case ITEM_ROTATION:
				animation->params.mode = EditMode::ITEM_ROTATION;
				animation->params.isDrawingMode = false;
				return 0;
			case ITEM_SCALE:
				animation->params.mode = EditMode::ITEM_SCALE;
				animation->params.isDrawingMode = false;
				return 0;
			case PREVIOUS_FRAME:
				animation->previous(hwnd);
				return 0;
			case NEXT_FRAME:
				animation->next(hwnd);
				return 0;
			case ADD_FRAME:
				animation->add(hwnd);
				return 0;
			case MID:
				animation->addMid(hwnd, 1);
				return 0;
			case COPY:
				animation = L::ToAnimation(L::ToString(animation));
				return 0;
			case OPEN:
			{
				OPENFILENAMEA openFileName;
				ZeroMemory(&openFileName, sizeof openFileName);
				openFileName.lStructSize = sizeof openFileName;
				openFileName.lpstrFilter = "2DsMax File\0*.2DsMax*\0";
				char* const lpstr = new char[20] { '2', 'D', 's', 'M', 'a', 'x', ' ', 'F', 'i', 'l', 'e', '\0', '.', '2', 'D', 's', 'M', 'a', 'x', '\0' };
				openFileName.nFilterIndex = 0;
				char* const lpstr1 = new char[256];
				openFileName.lpstrFile = lpstr1;
				openFileName.nMaxFile = 256;
				openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // Флаги
				if (GetOpenFileNameA(&openFileName))
				{
					std::ifstream fin;
					fin.open(openFileName.lpstrFile);
					size_t size = 0;
					char c;
					while (fin.get(c))
						size++;
					String s(size);
					size = 0;
					fin.clear();
					fin.seekg(0, std::ios::beg);
					while (fin.get(c))
					{
						s[size] = c;
						size++;
					}
					fin.close();
					delete animation;
					animation = L::ToAnimation(s);
					HDC hdc = GetDC(hwnd);
					animation->paint(hdc);
					ReleaseDC(hwnd, hdc);
				}
				return 0;
			}
			case SAVE_ANIMATION_AS:
			{
				OPENFILENAMEA openFileName;
				ZeroMemory(&openFileName, sizeof openFileName);
				openFileName.lStructSize = sizeof openFileName;
				openFileName.lpstrFilter = "2DsMax File\0*.2DsMax*\0";
				openFileName.nFilterIndex = 0;
				char* const lpstr1 = new char[256];
				openFileName.lpstrFile = lpstr1;
				openFileName.nMaxFile = 256;
				openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // Флаги
				if (GetSaveFileNameA(&openFileName))
				{
					std::ofstream fout;
					fout.open(openFileName.lpstrFile);
					String s = L::ToString(animation);
					for (char c : s)
					{
						fout << c;
					}
					fout.close();
				}
				return 0;
			}
			case DEL:
				animation->del(hwnd);
				return 0;
			}
			return 0;
		}
		HMENU get()
		{
			HMENU hmenu = CreateMenu();
			createFileMenu(hmenu);
			createEditMenu(hmenu);
			createNavigateMenu(hmenu);
			createModeMenu(hmenu);
			createSettingsMenu(hmenu);
			createShowFramesMenu(hmenu);
			return hmenu;
		}
		void createFileMenu(HMENU hmenu)
		{
			HMENU fileM = CreateMenu();
			HMENU saveFrameAsM = CreateMenu();
			AppendMenu(saveFrameAsM, MF_STRING, _2DSMAX_FILE, L"2DsMax Файл");
			AppendMenu(saveFrameAsM, MF_STRING, IMAGE, L"Изображение");
			AppendMenu(fileM, MF_STRING, NEW, L"Создать	Ctrl+N");
			AppendMenu(fileM, MF_STRING, OPEN, L"Открыть	Ctrl+O");
			AppendMenu(fileM, MF_POPUP, (UINT_PTR)saveFrameAsM, L"Сохранить кадр, как");
			AppendMenu(fileM, MF_STRING, SAVE_ANIMATION, L"Сохранить анимацию	Ctrl+S");
			AppendMenu(fileM, MF_STRING, SAVE_ANIMATION_AS, L"Сохранить анимацию, как...	Ctrl+Shift+S");
			AppendMenu(hmenu, MF_POPUP, (UINT_PTR)fileM, L"Файл");
		}
		void createEditMenu(HMENU hmenu)
		{
			HMENU editM = CreateMenu();
			AppendMenu(editM, MF_STRING, UNDO, L"Отменить	Ctrl+Z");
			AppendMenu(editM, MF_STRING, REDO, L"Вернуть	Shift+Ctrl+Z");
			AppendMenu(editM, MF_STRING, CUT, L"Вырезать	Ctrl+X");
			AppendMenu(editM, MF_STRING, COPY, L"Копировать	Ctrl+C");
			AppendMenu(editM, MF_STRING, PAST, L"Вставить	Ctrl+V");
			AppendMenu(editM, MF_STRING, DESELECT, L"Убрать выделение	Ctrl+D");
			AppendMenu(editM, MF_STRING, DEL, L"Удалить	Del");
			AppendMenu(editM, MF_STRING, MID, L"Добавить среднее	Ctrl+M");
			AppendMenu(hmenu, MF_POPUP, (UINT_PTR)editM, L"Правка");
		}
		void createNavigateMenu(HMENU hmenu)
		{
			HMENU navigateM = CreateMenu();
			AppendMenu(navigateM, MF_STRING, BACK, L"Назад	Ctrl+Left");
			AppendMenu(navigateM, MF_STRING, FORWARD, L"Вперёд	Ctrl+Right");
			AppendMenu(navigateM, MF_STRING, PREVIOUS_FRAME, L"Предыдущий кадр	Left");
			AppendMenu(navigateM, MF_STRING, NEXT_FRAME, L"Следующий кадр	Right");
			AppendMenu(navigateM, MF_STRING, CHOOSE_FRAME, L"Выбрать кадр");
			AppendMenu(navigateM, MF_STRING, ADD_FRAME, L"Добавить пустой кадр	Ctrl+E");
			AppendMenu(hmenu, MF_POPUP, (UINT_PTR)navigateM, L"Навигация");
		}
		void createModeMenu(HMENU hmenu)
		{
			HMENU modeM = CreateMenu();
			HMENU editM = CreateMenu();
			HMENU objM = CreateMenu();
			HMENU itemM = CreateMenu();
			HMENU centerM = CreateMenu();

			AppendMenu(objM, MF_STRING, LOCATION, L"Расположение");
			AppendMenu(objM, MF_STRING, ROTATION, L"Поворот");
			AppendMenu(objM, MF_STRING, SCALE, L"Размер");

			AppendMenu(itemM, MF_STRING, ITEM_LOCATION, L"Расположение частей");
			AppendMenu(itemM, MF_STRING, ITEM_ROTATION, L"Поворот частей");
			AppendMenu(itemM, MF_STRING, ITEM_SCALE, L"Размер частей");

			AppendMenu(centerM, MF_STRING, CENTER_LOCATION, L"Относительное расположение");
			AppendMenu(centerM, MF_STRING, CENTER_ROTATION, L"Относительный поворот");
			AppendMenu(centerM, MF_STRING, CENTER_SCALE, L"Относительный размер");

			AppendMenu(editM, MF_POPUP, (UINT_PTR)objM, L"объектов");
			AppendMenu(editM, MF_POPUP, (UINT_PTR)itemM, L"частей");
			AppendMenu(editM, MF_POPUP, (UINT_PTR)centerM, L"центров");

			AppendMenu(modeM, MF_POPUP, DRAW, L"Рисование");
			AppendMenu(modeM, MF_POPUP, (UINT_PTR)editM, L"Редактирование");
			AppendMenu(hmenu, MF_POPUP, (UINT_PTR)modeM, L"Режим");
		}
		void createSettingsMenu(HMENU hmenu)
		{
			HMENU settingsM = CreateMenu();
			AppendMenu(settingsM, MF_STRING, COLOR, L"Цвет");
			AppendMenu(settingsM, MF_STRING, WIDTH, L"Толщина");
			AppendMenu(hmenu, MF_POPUP, (UINT_PTR)settingsM, L"Настройки");
		}
		void createShowFramesMenu(HMENU hmenu)
		{
			HMENU showFramesM = CreateMenu();
			AppendMenu(showFramesM, MF_STRING, SHOW_AT_FIRST, L"Запустить с начала");
			AppendMenu(showFramesM, MF_STRING, STOP_SHOWING, L"Остановить");
			AppendMenu(showFramesM, MF_STRING, FRAME_RATE, L"Частота кадров");
			AppendMenu(hmenu, MF_POPUP, (UINT_PTR)showFramesM, L"Показ кадров");
		}
	};
}