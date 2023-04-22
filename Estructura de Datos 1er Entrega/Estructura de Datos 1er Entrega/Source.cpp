#include <iostream>
#include "resource.h"
#include <Windows.h>
#include <string>
#include <fstream>
#include <CommCtrl.h>
#include <regex>

using namespace std;

struct usuario {
	usuario* ant;
	char clave[50];
	char contra[50];
	char ApllPat[50];
	char ApllMat[50];
	char nombres[50];
	usuario* sig;
};

usuario* auxUsuario, *inicioUsuario, *miUsuario = nullptr;
void agregarUsuario(usuario* nuevo);
void escribirUsuarioBin();
void leerUsuarioBin();

struct persona {
	persona* ant;
	char ApllPat[50];
	char ApllMat[50];
	char nombres[50];
	char curp[25];
	char rfc[20];
	int dia = 0;
	int mes = 0;
	int año = 0;
	char calle[30];
	char colonia[30];
	char ciudad[20];
	char estado[30];
	char estadoCivil[20];
	char telefono[11];
	char docIdentidad[256];
	char sexo[12];
	char gpoOcupacional[20];
	char perfilRiesgo[25];
	char foto[256];
	persona* sig;
};

persona* auxPersona, * inicioPersona, * miPersona = nullptr;
void agregarPersona(persona* nuevo);
void modificarPersona(persona* newInfo, char curpPersona[25]);
void eliminarPersona(char curpPersona[25]);
void escribirPersonaBin();
void leerPersonaBin();
char personaTextoList[50] = {};
int countPersona = 0;

struct vacuna {
	vacuna* ant;
	char tipo[15];
	char marca[15];
	float numDosis = 0;
	char claveVacuna[20];
	char descripcion[50];
	float precio = 0;
	vacuna* sig;
};

vacuna* auxVacuna, *inicioVacuna, *miVacuna = nullptr;
void agregarVacuna(vacuna* nuevo);
void modificarVacuna(vacuna* newInfo, char claveVacuna[20]);
void escribirVacunaBin();
void eliminarVacuna(char claveVacuna[20]);
void leerVacunasBin();
char vacunaTextoList[50] = {};
int countVacuna = 0;

struct carnet {
	carnet* ant;
	char curp[25];
	char vacuna[50];
	int dosis = 0;
	int dia = 0;
	int mes = 0;
	int año = 0;
	char lote[15];
	char centroVacuna[50];
	char foto[256];
	int ID = 0;
	carnet* sig;
};

carnet* auxCarnet, * inicioCarnet, *medio, *auxCarnet2 = nullptr;
void agregarCarnet(carnet* nuevo);
int busquedaBinaria(carnet* inicio, carnet* fin, int buscar);
void modificarCarnet(carnet* newInfo,char curp[25]);
void eliminarCarnet(char curp[25]);
void escribirCarnetBin();
void leerCarnetBin();
char carnetCurpList[25] = {}, carnetVacunaList[10] = {};
char carnetListaCurp[50] = {};
int countCarnet1 = 0, countCarnet2 = 0, countCarnet3 = 0;

char zFile[MAX_PATH];
char yFile[MAX_PATH];

bool selectCurp = false, selectVacuna = false;
bool busquedaBin = false;

LRESULT CALLBACK procVInicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVMenu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVRegistrarPersona(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVListaPersona(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVRegistrarVacuna(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVListaVacuna(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVRegistrarCarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVListaCarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInstGlobal;
HWND hInstMenu;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
	leerUsuarioBin();
	leerPersonaBin();
	leerVacunasBin();
	leerCarnetBin();
	hInstGlobal = hInst;
	HWND hVInicio = CreateDialog(hInst, MAKEINTRESOURCE(101), NULL, procVInicio);

	ShowWindow(hVInicio, cmdshow);

	MSG Mensaje;
	ZeroMemory(&Mensaje, sizeof(Mensaje));

	while (GetMessage(&Mensaje, 0, 0, 0)) {
		TranslateMessage(&Mensaje);
		DispatchMessage(&Mensaje);
	}
	escribirUsuarioBin();
	escribirPersonaBin();
	escribirVacunaBin();
	escribirCarnetBin();
	return Mensaje.wParam;
}

LRESULT CALLBACK procVInicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_bIniciar: {
			char userClaveChar[50];
			char userContraChar[50];
			GetDlgItemText(hwnd, ID_InicioSesClave, userClaveChar, sizeof(userClaveChar));
			GetDlgItemText(hwnd, ID_InicioSesContra, userContraChar, sizeof(userContraChar));
			string userClave = userClaveChar;
			string userContra = userContraChar;

			auxUsuario = inicioUsuario;
			if (inicioUsuario == nullptr) {
				MessageBox(hwnd, "No hay usuario registrados.", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {
				while (auxUsuario != nullptr && auxUsuario->clave != userClave) {
					auxUsuario = auxUsuario->sig;
				}
				if (auxUsuario == nullptr) {
					MessageBox(hwnd, "Usuario no encontrado", "AVISO", MB_OK | MB_ICONERROR);
				}
				else {
					if (auxUsuario->contra == userContra) {
						miUsuario = auxUsuario;
						EndDialog(hwnd, 0);
						DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_Menu), hwnd, procVMenu);
					}
					else {
						MessageBox(hwnd, "Contraseña incorrecta", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
			}
		} break;
		case ID_bRegistrarse: {
			bool validClave = true, validContra1 = false, validContra2 = false;
			usuario* newUsuario = new usuario;
			GetDlgItemText(hwnd, ID_RegistrarClave, newUsuario->clave, sizeof(newUsuario->clave));
			GetDlgItemText(hwnd, ID_RegistrarContra, newUsuario->contra, sizeof(newUsuario->contra));
			GetDlgItemText(hwnd, ID_RegistrarApllPat, newUsuario->ApllPat, sizeof(newUsuario->ApllPat));
			GetDlgItemText(hwnd, ID_RegistrarApllMat, newUsuario->ApllMat, sizeof(newUsuario->ApllMat));
			GetDlgItemText(hwnd, ID_RegistrarNombre, newUsuario->nombres, sizeof(newUsuario->nombres));

			if (newUsuario->clave != "" && newUsuario->contra != "" && newUsuario->ApllPat != "" && newUsuario->ApllMat != "" && newUsuario->nombres != "") {
				auxUsuario = inicioUsuario;
				while (auxUsuario != nullptr) {
					if (strcmp(auxUsuario->clave, newUsuario->clave) == 0) {
						validClave = false;
						break;
					}
					else {
						validClave = true;
						auxUsuario = auxUsuario->sig;
					}
				}
				if (validClave == true) {
					agregarUsuario(newUsuario);
					delete (newUsuario);
					SetDlgItemText(hwnd, ID_RegistrarClave, "");
					SetDlgItemText(hwnd, ID_RegistrarContra, "");
					SetDlgItemText(hwnd, ID_RegistrarApllPat, "");
					SetDlgItemText(hwnd, ID_RegistrarApllPat, "");
					SetDlgItemText(hwnd, ID_RegistrarApllMat, "");
					SetDlgItemText(hwnd, ID_RegistrarNombre, "");
				}
				else {
					MessageBox(hwnd, "Esa clave de usuario ya esta siendo usada", "AVISO", MB_OK | MB_ICONERROR);
				}
			}
			else {
				MessageBox(hwnd, "No pueden haber campos vacios", "AVISO", MB_OK | MB_ICONERROR);
			}
		} break;
		}
	}break;
	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		if (opc == IDYES) {
			DestroyWindow(hwnd);
		}
	}break;
	case WM_DESTROY: {
		PostQuitMessage(0);
	}break;
	}
	return false;
}

LRESULT CALLBACK procVMenu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_Salir: {
			int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			if (opc == IDYES) {
				DestroyWindow(hwnd);
			}
		}break;
		case ID_PERSONAS_REGISTRAR: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_RegisPersona), hwnd, procVRegistrarPersona);
		}break;
		case ID_PERSONAS_LISTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_ListaPersona), hwnd, procVListaPersona);
		}break;
		case ID_VACUNAS_REGISTRAR: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_RegistrarVacuna), hwnd, procVRegistrarVacuna);
		}break;
		case ID_VACUNAS_LISTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_ListaVacuna), hwnd, procVListaVacuna);
		}break;
		case ID_CARNET_REGISTRAR: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_RegistrarCarnet), hwnd, procVRegistrarCarnet);
		}break;
		case ID_CARNET_LISTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_ListaCarnet), hwnd, procVListaCarnet);
		}break;
		}
	}break;
	case WM_DESTROY: {
		PostQuitMessage(0);
	}break;
	}
	return false;
}

LRESULT CALLBACK procVRegistrarPersona(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	SYSTEMTIME RegisFecha;
	switch (msg) {
	case WM_INITDIALOG: {
		char sexo[2][10] = { "Masculino","Femenino" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, ID_RegisPersonaSexo, CB_INSERTSTRING, (WPARAM)i, (LPARAM)sexo[i]);
		}
		char EstadoCivil[6][15] = { "Casado","Conviviente","Divorciado","Separado","Viudo","Soltero" };
		char PerfilRiesgo[6][22] = { "Ninguno","Embarazada","Menor de 5 años","Enfermedad cronica","Obesidad o sobrepeso","Mayor de 60 años" };
		char GrupoOcupacional[6][16] = { "Doctor","Ingeniero", "Maestro", "Piloto", "Secretari@","Artista" };
		for (int j = 0; j < 6; j++) {
			SendDlgItemMessage(hwnd, ID_RegisPersonaEstCivil, CB_INSERTSTRING, (WPARAM)j, (LPARAM)EstadoCivil[j]);
			SendDlgItemMessage(hwnd, ID_RegisPersonaPrflRiesgo, CB_INSERTSTRING, (WPARAM)j, (WPARAM)PerfilRiesgo[j]);
			SendDlgItemMessage(hwnd, ID_RegisPersonaGpoOcpcnl, CB_INSERTSTRING, (WPARAM)j, (LPARAM)GrupoOcupacional[j]);
		}
		SendMessage(GetDlgItem(hwnd, ID_RegisPersonaSexo), CB_SETCURSEL, (WPARAM)0, 0);
		SendMessage(GetDlgItem(hwnd, ID_RegisPersonaGpoOcpcnl), CB_SETCURSEL, (WPARAM)0, 0);
		SendMessage(GetDlgItem(hwnd, ID_RegisPersonaEstCivil), CB_SETCURSEL, (WPARAM)0, 0);
		SendMessage(GetDlgItem(hwnd, ID_RegisPersonaPrflRiesgo), CB_SETCURSEL, (WPARAM)0, 0);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_bRegisPersonaFoto: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp;

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendDlgItemMessage(hwnd, ID_RegisPersonaFoto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}
			else {
				MessageBox(hwnd, "No seleccionó foto", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_bRegisPersonaDocumento: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = yFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(yFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp;

				bmp = (HBITMAP)LoadImage(NULL, yFile, IMAGE_BITMAP, 175, 100, LR_LOADFROMFILE);
				SendDlgItemMessage(hwnd, ID_RegisPersonaDocumento, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}
			else {
				MessageBox(hwnd, "No seleccionó foto", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_bRegistrarPersona: {
			bool validCURP = false, validRFC = false, validCURP2 = true, validRFC2 = true;
			persona* newPersona = new persona;
			strcpy_s(newPersona->foto, zFile);
			strcpy_s(newPersona->docIdentidad, yFile);
			GetDlgItemText(hwnd, ID_RegisPersonaApllPat, newPersona->ApllPat, sizeof(newPersona->ApllPat));
			GetDlgItemText(hwnd, ID_RegisPersonaApllMat, newPersona->ApllMat, sizeof(newPersona->ApllMat));
			GetDlgItemText(hwnd, ID_RegisPersonaNombres, newPersona->nombres, sizeof(newPersona->nombres));
			GetDlgItemText(hwnd, ID_RegisPersonaCurp, newPersona->curp, sizeof(newPersona->curp));
			GetDlgItemText(hwnd, ID_RegisPersonaRFC, newPersona->rfc, sizeof(newPersona->rfc));
			GetDlgItemText(hwnd, ID_RegisPersonaCalle, newPersona->calle, sizeof(newPersona->calle));
			GetDlgItemText(hwnd, ID_RegisPersonaColonia, newPersona->colonia, sizeof(newPersona->colonia));
			GetDlgItemText(hwnd, ID_RegisPersonaCiudad, newPersona->ciudad, sizeof(newPersona->ciudad));
			GetDlgItemText(hwnd, ID_RegisPersonaestado, newPersona->estado, sizeof(newPersona->estado));
			GetDlgItemText(hwnd, ID_RegisPersonaTelefono, newPersona->telefono, sizeof(newPersona->telefono));
			GetDlgItemText(hwnd, ID_RegisPersonaSexo, newPersona->sexo, sizeof(newPersona->sexo));
			GetDlgItemText(hwnd, ID_RegisPersonaGpoOcpcnl, newPersona->gpoOcupacional, sizeof(newPersona->gpoOcupacional));
			GetDlgItemText(hwnd, ID_RegisPersonaPrflRiesgo, newPersona->perfilRiesgo, sizeof(newPersona->perfilRiesgo));
			GetDlgItemText(hwnd, ID_RegisPersonaEstCivil, newPersona->estadoCivil, sizeof(newPersona->estadoCivil));
			SendDlgItemMessage(hwnd, ID_RegisPersonaFechaNacimiento, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (RegisFecha));
			newPersona->dia = RegisFecha.wDay;
			newPersona->mes = RegisFecha.wMonth;
			newPersona->año = RegisFecha.wYear;

			if (strlen(newPersona->curp) != 18) {
				validCURP = false;
			}
			else {
				validCURP = true;
			}

			if (strlen(newPersona->rfc) != 13) {
				validRFC = false;
			}
			else {
				validRFC = true;
			}
			if (validCURP == true) {
				if (validRFC == true) {
					if (newPersona->ApllPat != "" && newPersona->ApllMat != "" && newPersona->nombres != "" && newPersona->curp != "" && newPersona->rfc != "" && newPersona->telefono != "" && newPersona->docIdentidad != "" && newPersona->foto != "") {
						auxPersona = inicioPersona;
						while (auxPersona != nullptr) {
							if (strcmp(auxPersona->curp, newPersona->curp) == 0) {
								validCURP2 = false;
								break;
							}
							else {
								validCURP2 = true;
								auxPersona = auxPersona->sig;
							}
						}
						
						auxPersona = inicioPersona;
						while (auxPersona != nullptr) {
							if (strcmp(auxPersona->rfc, newPersona->rfc) == 0) {
								validRFC2 = false;
								break;
							}
							else {
								validRFC2 = true;
								auxPersona = auxPersona->sig;
							}
						}

						if (validCURP2 == true) {
							if (validRFC2 == true) {
								agregarPersona(newPersona);
								delete(newPersona);
								SetDlgItemText(hwnd, ID_RegisPersonaApllPat, "");
								SetDlgItemText(hwnd, ID_RegisPersonaApllMat, "");
								SetDlgItemText(hwnd, ID_RegisPersonaNombres, "");
								SetDlgItemText(hwnd, ID_RegisPersonaCurp, "");
								SetDlgItemText(hwnd, ID_RegisPersonaRFC, "");
								SetDlgItemText(hwnd, ID_RegisPersonaTelefono, "");
								SetDlgItemText(hwnd, ID_RegisPersonaCalle, "");
								SetDlgItemText(hwnd, ID_RegisPersonaColonia, "");
								SetDlgItemText(hwnd, ID_RegisPersonaCiudad, "");
								SetDlgItemText(hwnd, ID_RegisPersonaestado, "");
								SetDlgItemText(hwnd, ID_RegisPersonaDocumento, "");
								SetDlgItemText(hwnd, ID_RegisPersonaFoto, "");
							}
							else {
								MessageBox(hwnd, "Ese RFC ya esta en uso", "AVISO", MB_OK | MB_ICONERROR);
							}
						}
						else {
							MessageBox(hwnd, "Ese CURP ya esta en uso", "AVISO", MB_OK | MB_ICONERROR);
						}
					}
					else {
						MessageBox(hwnd, "No pueden haber campos vacios", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
				else {
					MessageBox(hwnd, "La RFC tiene que ser de 13 caracteres", "AVISO", MB_OK | MB_ICONERROR);
				}	
			}
			else {
				MessageBox(hwnd, "La CURP tiene que ser de 18 caracteres", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_bRegisPersonaSalir: {
			EndDialog(hwnd, 0);
		}break;
		}
	}break;
	}
	return false;
}

LRESULT CALLBACK procVListaPersona(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		char sexo[2][10] = { "Masculino","Femenino" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, ID_ListaPersonaSexo, CB_INSERTSTRING, (WPARAM)i, (LPARAM)sexo[i]);
		}
		char EstadoCivil[6][15] = { "Casado","Conviviente","Divorciado","Separado","Viudo","Soltero" };
		char PerfilRiesgo[6][22] = { "Ninguno","Embarazada","Menor de 5 años","Enfermedad cronica","Obesidad o sobrepeso","Mayor de 60 años" };
		char GrupoOcupacional[6][16] = { "Doctor","Ingeniero", "Maestro", "Piloto", "Secretari@","Artista" };
		for (int j = 0; j < 6; j++) {
			SendDlgItemMessage(hwnd, ID_ListaPersonaEstCivil, CB_INSERTSTRING, (WPARAM)j, (LPARAM)EstadoCivil[j]);
			SendDlgItemMessage(hwnd, ID_ListaPersonaPrflRiesgo, CB_INSERTSTRING, (WPARAM)j, (WPARAM)PerfilRiesgo[j]);
			SendDlgItemMessage(hwnd, ID_ListaPersonaGpoOcpcnl, CB_INSERTSTRING, (WPARAM)j, (LPARAM)GrupoOcupacional[j]);
		}
		SendMessage(GetDlgItem(hwnd, ID_ListaPersonaSexo), CB_SETCURSEL, (WPARAM)0, 0);
		SendMessage(GetDlgItem(hwnd, ID_ListaPersonaGpoOcpcnl), CB_SETCURSEL, (WPARAM)0, 0);
		SendMessage(GetDlgItem(hwnd, ID_ListaPersonaEstCivil), CB_SETCURSEL, (WPARAM)0, 0);
		SendMessage(GetDlgItem(hwnd, ID_ListaPersonaPrflRiesgo), CB_SETCURSEL, (WPARAM)0, 0);
		auxPersona = inicioPersona;
		while (auxPersona != nullptr) {
			SendMessage(GetDlgItem(hwnd, ID_ListaListPersona), LB_ADDSTRING, 0, (LPARAM)auxPersona->curp);
			auxPersona = auxPersona->sig;
		}
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_ListaListPersona: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				SYSTEMTIME ListaFecha;
				countPersona = SendDlgItemMessage(hwnd, ID_ListaListPersona, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, ID_ListaListPersona, LB_GETTEXT, countPersona, (LPARAM)personaTextoList);
				auxPersona = inicioPersona;
				while (auxPersona != nullptr && strcmp(personaTextoList, auxPersona->curp) != 0) {
					auxPersona = auxPersona->sig;
				}
				if (strcmp(personaTextoList, auxPersona->curp) == 0) {
					SetDlgItemText(hwnd, ID_ListaPersonaApllPat, auxPersona->ApllPat);
					SetDlgItemText(hwnd, ID_ListaPersonaApllMat, auxPersona->ApllMat);
					SetDlgItemText(hwnd, ID_ListaPersonaNombres, auxPersona->nombres);
					SetDlgItemText(hwnd, ID_ListaPersonaCurp, auxPersona->curp);
					SetDlgItemText(hwnd, ID_ListaPersonaRFC, auxPersona->rfc);
					ListaFecha.wDay = auxPersona->dia;
					ListaFecha.wMonth = auxPersona->mes;
					ListaFecha.wYear = auxPersona->año;
					ListaFecha.wMilliseconds = 0;
					ListaFecha.wSecond = 0;
					ListaFecha.wMinute = 0;
					ListaFecha.wHour = 0;
					SendDlgItemMessage(hwnd, ID_ListaPersonaFechaNacimiento, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM) &ListaFecha);
					SetDlgItemText(hwnd, ID_ListaPersonaCalle, auxPersona->calle);
					SetDlgItemText(hwnd, ID_ListaPersonaColonia, auxPersona->colonia);
					SetDlgItemText(hwnd, ID_ListaPersonaCiudad, auxPersona->ciudad);
					SetDlgItemText(hwnd, ID_ListaPersonaestado, auxPersona->estado);
					SendDlgItemMessage(hwnd, ID_ListaPersonaEstCivil, CB_SELECTSTRING, 0, (LPARAM)auxPersona->estadoCivil);
					SetDlgItemText(hwnd, ID_ListaPersonaTelefono, auxPersona->telefono);
					SendDlgItemMessage(hwnd, ID_ListaPersonaSexo, CB_SELECTSTRING, 0, (LPARAM)auxPersona->sexo);
					SendDlgItemMessage(hwnd, ID_ListaPersonaGpoOcpcnl, CB_SELECTSTRING, 0, (LPARAM)auxPersona->gpoOcupacional);
					SendDlgItemMessage(hwnd, ID_ListaPersonaPrflRiesgo, CB_SELECTSTRING, 0, (LPARAM)auxPersona->perfilRiesgo);
					HBITMAP bmp;
					HBITMAP bmp2;
					bmp = (HBITMAP)LoadImage(NULL, auxPersona->foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					SendDlgItemMessage(hwnd, ID_ListaPersonaFoto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
					bmp2 = (HBITMAP)LoadImage(NULL, auxPersona->docIdentidad, IMAGE_BITMAP, 175, 100, LR_LOADFROMFILE);
					SendDlgItemMessage(hwnd, ID_ListaPersonaDocumento, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
					miPersona = auxPersona;
					EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaApllPat), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaApllMat), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaNombres), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaFechaNacimiento), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaCalle), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaColonia), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaCiudad), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaestado), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaEstCivil), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaTelefono), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaSexo), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaGpoOcpcnl), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaPrflRiesgo), true);
					EnableWindow(GetDlgItem(hwnd, ID_bListaPersonaFoto), true);
					EnableWindow(GetDlgItem(hwnd, ID_bListaPersonaDocumento), true);
					EnableWindow(GetDlgItem(hwnd, ID_bListaPersonaModif), true);
					EnableWindow(GetDlgItem(hwnd, ID_bListaPersonaElim), true);
				}
			}break;
			}
		}break;
		case ID_bListaPersonaFoto: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp;

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendDlgItemMessage(hwnd, ID_ListaPersonaFoto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}
			else {
				MessageBox(hwnd, "No seleccionó foto", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_bListaPersonaDocumento: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = yFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(yFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp;

				bmp = (HBITMAP)LoadImage(NULL, yFile, IMAGE_BITMAP, 175, 100, LR_LOADFROMFILE);
				SendDlgItemMessage(hwnd, ID_ListaPersonaDocumento, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}
			else {
				MessageBox(hwnd, "No seleccionó foto", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_bListaPersonaModif: {
			SYSTEMTIME ListaFecha;
			bool validCURP = false, validRFC = false, validCURP2 = true, validRFC2 = true;
			persona* newInfoPersona = new persona;
			GetDlgItemText(hwnd, ID_ListaPersonaApllPat, newInfoPersona->ApllPat, sizeof(newInfoPersona->ApllPat));
			GetDlgItemText(hwnd, ID_ListaPersonaApllMat, newInfoPersona->ApllMat, sizeof(newInfoPersona->ApllMat));
			GetDlgItemText(hwnd, ID_ListaPersonaNombres, newInfoPersona->nombres, sizeof(newInfoPersona->nombres));
			GetDlgItemText(hwnd, ID_ListaPersonaCurp, newInfoPersona->curp, sizeof(newInfoPersona->curp));
			GetDlgItemText(hwnd, ID_ListaPersonaRFC, newInfoPersona->rfc, sizeof(newInfoPersona->rfc));
			SendDlgItemMessage(hwnd, ID_ListaPersonaFechaNacimiento, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (ListaFecha));
			newInfoPersona->dia = ListaFecha.wDay;
			newInfoPersona->mes = ListaFecha.wMonth;
			newInfoPersona->año = ListaFecha.wYear;
			GetDlgItemText(hwnd, ID_ListaPersonaCalle, newInfoPersona->calle, sizeof(newInfoPersona->calle));
			GetDlgItemText(hwnd, ID_ListaPersonaColonia, newInfoPersona->colonia, sizeof(newInfoPersona->colonia));
			GetDlgItemText(hwnd, ID_ListaPersonaCiudad, newInfoPersona->ciudad, sizeof(newInfoPersona->ciudad));
			GetDlgItemText(hwnd, ID_ListaPersonaestado, newInfoPersona->estado, sizeof(newInfoPersona->estado));
			GetDlgItemText(hwnd, ID_ListaPersonaEstCivil, newInfoPersona->estadoCivil, sizeof(newInfoPersona->estadoCivil));
			GetDlgItemText(hwnd, ID_ListaPersonaTelefono, newInfoPersona->telefono, sizeof(newInfoPersona->telefono));
			GetDlgItemText(hwnd, ID_ListaPersonaSexo, newInfoPersona->sexo, sizeof(newInfoPersona->sexo));
			GetDlgItemText(hwnd, ID_ListaPersonaGpoOcpcnl, newInfoPersona->gpoOcupacional, sizeof(newInfoPersona->gpoOcupacional));
			GetDlgItemText(hwnd, ID_ListaPersonaPrflRiesgo, newInfoPersona->perfilRiesgo, sizeof(newInfoPersona->perfilRiesgo));
			strcpy_s(newInfoPersona->foto, zFile);
			strcpy_s(newInfoPersona->docIdentidad, yFile);

			if (strlen(newInfoPersona->curp) != 18) {
				validCURP = false;
			}
			else {
				validCURP = true;
			}

			if (strlen(newInfoPersona->rfc) != 13) {
				validRFC = false;
			}
			else {
				validRFC = true;
			}

			if (validCURP == true) {
				if (validRFC == true) {
					if (newInfoPersona->ApllPat != "" && newInfoPersona->ApllMat != "" && newInfoPersona->nombres != "" && newInfoPersona->curp != "" && newInfoPersona->rfc != "" && newInfoPersona->telefono != "" && newInfoPersona->docIdentidad != "" && newInfoPersona->foto != "") {
						auxPersona = inicioPersona;
						while (auxPersona != nullptr) {
							if (strcmp(newInfoPersona->curp, auxPersona->curp) == 0) {
								if (strcmp(newInfoPersona->curp, miPersona->curp) == 0) {
									validCURP2 = true;
								}
								else {
									validCURP2 = false;
									break;
								}
							}
							else {
								validCURP2 = true;
								auxPersona = auxPersona->sig;
							}
						}

						auxPersona = inicioPersona;
						while (auxPersona != nullptr) {
							if (strcmp(newInfoPersona->rfc, auxPersona->rfc) == 0) {
								if (strcmp(newInfoPersona->rfc, miPersona->rfc) == 0) {
									validRFC2 = true;
								}
								else {
									validRFC2 = false;
									break;
								}	
							}
							else {
								validRFC2 = true;
								auxPersona = auxPersona->sig;
							}
						}

						if (validCURP2 == true) {
							if (validRFC2 == true) {
								modificarPersona(newInfoPersona, personaTextoList);
								delete newInfoPersona;
								SetDlgItemText(hwnd, ID_ListaPersonaApllPat, "");
								SetDlgItemText(hwnd, ID_ListaPersonaApllMat, "");
								SetDlgItemText(hwnd, ID_ListaPersonaNombres, "");
								SetDlgItemText(hwnd, ID_ListaPersonaCurp, "");
								SetDlgItemText(hwnd, ID_ListaPersonaRFC, "");
								SetDlgItemText(hwnd, ID_ListaPersonaTelefono, "");
								SetDlgItemText(hwnd, ID_ListaPersonaCalle, "");
								SetDlgItemText(hwnd, ID_ListaPersonaColonia, "");
								SetDlgItemText(hwnd, ID_ListaPersonaCiudad, "");
								SetDlgItemText(hwnd, ID_ListaPersonaestado, "");
								SetDlgItemText(hwnd, ID_ListaPersonaDocumento, "");
								SetDlgItemText(hwnd, ID_ListaPersonaFoto, "");
								EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaApllPat), false);
								EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaApllMat), false);
								EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaNombres), false);
								EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaFechaNacimiento), false);
								EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaCalle), false);
								EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaColonia), false);
								EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaCiudad), false);
								EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaestado), false);
								EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaEstCivil), false);
								EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaTelefono), false);
								EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaSexo), false);
								EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaGpoOcpcnl), false);
								EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaPrflRiesgo), false);
								EnableWindow(GetDlgItem(hwnd, ID_bListaPersonaFoto), false);
								EnableWindow(GetDlgItem(hwnd, ID_bListaPersonaDocumento), false);
								EnableWindow(GetDlgItem(hwnd, ID_bListaPersonaElim), false);
								EnableWindow(GetDlgItem(hwnd, ID_bListaPersonaModif), false);
								SendMessage(GetDlgItem(hwnd, ID_ListaListPersona), LB_RESETCONTENT, 0, 0);
								auxPersona = inicioPersona;
								while (auxPersona != nullptr) {
									SendMessage(GetDlgItem(hwnd, ID_ListaListPersona), LB_ADDSTRING, 0, (LPARAM)auxPersona->curp);
									auxPersona = auxPersona->sig;
								}
							}
							else {
								MessageBox(hwnd, "Ese RFC ya esta en uso", "AVISO", MB_OK | MB_ICONERROR);
							}
						}
						else {
							MessageBox(hwnd, "Ese CURP ya esta en uso", "AVISO", MB_OK | MB_ICONERROR);
						}
					}
					else {
						MessageBox(hwnd, "No pueden haber campos vacios", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
				else {
					MessageBox(hwnd, "El RFC tiene que ser de 13 caracteres", "AVISO", MB_OK | MB_ICONERROR);
				}
			}
			else {
				MessageBox(hwnd, "El CURP tiene que ser de 13 caracteres", "AVISO", MB_OK | MB_ICONERROR);
			}		
		}break;
		case ID_bListaPersonaElim: {
			int opc = MessageBox(hwnd, "¿Seguro que desea eliminar a esta persona?", "AVISO", MB_OKCANCEL | MB_ICONQUESTION);
			if (opc == IDOK) {
				eliminarPersona(personaTextoList);
				SetDlgItemText(hwnd, ID_ListaPersonaApllPat, "");
				SetDlgItemText(hwnd, ID_ListaPersonaApllMat, "");
				SetDlgItemText(hwnd, ID_ListaPersonaNombres, "");
				SetDlgItemText(hwnd, ID_ListaPersonaCurp, "");
				SetDlgItemText(hwnd, ID_ListaPersonaRFC, "");
				SetDlgItemText(hwnd, ID_ListaPersonaTelefono, "");
				SetDlgItemText(hwnd, ID_ListaPersonaCalle, "");
				SetDlgItemText(hwnd, ID_ListaPersonaColonia, "");
				SetDlgItemText(hwnd, ID_ListaPersonaCiudad, "");
				SetDlgItemText(hwnd, ID_ListaPersonaestado, "");
				SetDlgItemText(hwnd, ID_ListaPersonaDocumento, "");
				SetDlgItemText(hwnd, ID_ListaPersonaFoto, "");
				EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaApllPat), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaApllMat), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaNombres), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaFechaNacimiento), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaCalle), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaColonia), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaCiudad), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaestado), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaEstCivil), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaTelefono), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaSexo), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaGpoOcpcnl), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaPersonaPrflRiesgo), false);
				EnableWindow(GetDlgItem(hwnd, ID_bListaPersonaFoto), false);
				EnableWindow(GetDlgItem(hwnd, ID_bListaPersonaDocumento), false);
				EnableWindow(GetDlgItem(hwnd, ID_bListaPersonaElim), false);
				EnableWindow(GetDlgItem(hwnd, ID_bListaPersonaModif), false);
				SendMessage(GetDlgItem(hwnd, ID_ListaListPersona), LB_RESETCONTENT, 0, 0);
				auxPersona = inicioPersona;
				while (auxPersona != nullptr) {
					SendMessage(GetDlgItem(hwnd, ID_ListaListPersona), LB_ADDSTRING, 0, (LPARAM)auxPersona->curp);
					auxPersona = auxPersona->sig;
				}
			}
		}break;
		case ID_bListaPersonaSalir: {
			EndDialog(hwnd, 0);
		}break;
		}
	}break;
	}
	return false;
}

LRESULT CALLBACK procVRegistrarVacuna(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char RegisPrecio[10] = "asd", RegisDosis[4] = "asd";
	bool ValidPrecio = false, ValidClave = true;
	switch (msg) {
	case WM_INITDIALOG: {
		char Tipo[3][30] = { "ARN","Vectorviral","Subunidades proteicas" };
		char Marca[4][15] = { "Pfizer","AstraZenaca","Casino","Moderna" };
		for (int i = 0; i < 3; i++) {
			SendDlgItemMessage(hwnd, ID_RegisVacunaTipo, CB_INSERTSTRING, (WPARAM)i, (LPARAM)Tipo[i]);
		}
		for (int j = 0; j < 4; j++) {
			SendDlgItemMessage(hwnd, ID_RegisVacunaMarca, CB_INSERTSTRING, (WPARAM)j, (LPARAM)Marca[j]);
		}
		SendMessage(GetDlgItem(hwnd, ID_RegisVacunaTipo), CB_SETCURSEL, (WPARAM)0, 0);
		SendMessage(GetDlgItem(hwnd, ID_RegisVacunaMarca), CB_SETCURSEL, (WPARAM)0, 0);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_bRegistrarVacuna: {
			vacuna* newVacuna = new vacuna;
			GetDlgItemText(hwnd, ID_RegisVacunaTipo, newVacuna->tipo, sizeof(newVacuna->tipo));
			GetDlgItemText(hwnd, ID_RegisVacunaMarca, newVacuna->marca, sizeof(newVacuna->marca));
			GetDlgItemText(hwnd, ID_RegisVacunaNo, RegisDosis, sizeof(RegisDosis));
			newVacuna->numDosis = atoi(RegisDosis);
			GetDlgItemText(hwnd, ID_RegisVacunaClave, newVacuna->claveVacuna, sizeof(newVacuna->claveVacuna));
			GetDlgItemText(hwnd, ID_RegisVacunaDesc, newVacuna->descripcion, sizeof(newVacuna->descripcion));
			GetDlgItemText(hwnd, ID_RegisVacunaPrecio, RegisPrecio, sizeof(RegisPrecio));
			newVacuna->precio = atof(RegisPrecio);

			if (newVacuna->precio > 0) {
				ValidPrecio = true;
			}
			else {
				ValidPrecio = false;
			}

			auxVacuna = inicioVacuna;
			while (auxVacuna != nullptr) {
				if (auxVacuna->claveVacuna == newVacuna->claveVacuna) {
					ValidClave = false;
					break;
				}
				else {
					ValidClave = true;
					auxVacuna = auxVacuna->sig;
				}
			}

			if (newVacuna->numDosis > 0 && newVacuna->claveVacuna != "" && newVacuna->descripcion != "") {
				if (ValidPrecio == true) {
					if (ValidClave == true) {
						agregarVacuna(newVacuna);
						delete(newVacuna);
						SetDlgItemText(hwnd, ID_RegisVacunaNo, "");
						SetDlgItemText(hwnd, ID_RegisVacunaClave, "");
						SetDlgItemText(hwnd, ID_RegisVacunaDesc, "");
						SetDlgItemText(hwnd, ID_RegisVacunaPrecio, "");
					}
					else {
						MessageBox(hwnd, "Esa clave ya esta en uso", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
				else {
					MessageBox(hwnd, "El precio tiene que ser mayor que 0", "AVISO", MB_OK | MB_ICONERROR);
				}
			}
			else {
				MessageBox(hwnd, "No pueden haber campos vacios", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_bRegisVacunaSalir: {
			EndDialog(hwnd, 0);
		}break;
		}
	}break;
	}

	return false;
}

LRESULT CALLBACK procVListaVacuna(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char auxVacunaPrecio[20] = "asd", auxDosis[20] = "asd";
	char auxVacunaPrecio2[20] = "asd", auxDosis2[20] = "asd";
	bool ValidPrecio = false, ValidClave = true;
	switch (msg) {
	case WM_INITDIALOG: {
		char Tipo[3][30] = { "ARN","Vectorviral","Subunidades proteicas" };
		char Marca[4][15] = { "Pfizer","AstraZenaca","Casino","Moderna" };
		for (int i = 0; i < 3; i++) {
			SendDlgItemMessage(hwnd, ID_ListaVacunaTipo, CB_INSERTSTRING, (WPARAM)i, (LPARAM)Tipo[i]);
		}
		for (int j = 0; j < 4; j++) {
			SendDlgItemMessage(hwnd, ID_ListaVacunaMarca, CB_INSERTSTRING, (WPARAM)j, (LPARAM)Marca[j]);
		}
		SendMessage(GetDlgItem(hwnd, ID_ListaVacunaTipo), CB_SETCURSEL, (WPARAM)0, 0);
		SendMessage(GetDlgItem(hwnd, ID_ListaVacunaMarca), CB_SETCURSEL, (WPARAM)0, 0);
		auxVacuna = inicioVacuna;
		while (auxVacuna != nullptr) {
			SendMessage(GetDlgItem(hwnd, ID_ListaListVacuna), LB_ADDSTRING, 0, (LPARAM)auxVacuna->claveVacuna);
			auxVacuna = auxVacuna->sig;
		}
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_ListaListVacuna: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				countVacuna = SendDlgItemMessage(hwnd, ID_ListaListVacuna, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, ID_ListaListVacuna, LB_GETTEXT, countVacuna, (LPARAM)vacunaTextoList);
				auxVacuna = inicioVacuna;
				while (auxVacuna != nullptr && strcmp(vacunaTextoList, auxVacuna->claveVacuna) != 0) {
					auxVacuna = auxVacuna->sig;
				}
				if (strcmp(vacunaTextoList, auxVacuna->claveVacuna) == 0) {
					SendDlgItemMessage(hwnd, ID_ListaVacunaTipo, CB_SELECTSTRING, 0, (LPARAM)auxVacuna->tipo);
					SendDlgItemMessage(hwnd, ID_ListaVacunaMarca, CB_SELECTSTRING, 0, (LPARAM)auxVacuna->marca);
					sprintf_s(auxDosis, "%f", auxVacuna->numDosis);
					SetDlgItemText(hwnd, ID_ListaVacunaNo, auxDosis);
					SetDlgItemText(hwnd, ID_ListaVacunaClave, auxVacuna->claveVacuna);
					SetDlgItemText(hwnd, ID_ListaVacunaDesc, auxVacuna->descripcion);
					sprintf_s(auxVacunaPrecio, "%f", auxVacuna->precio);
					SetDlgItemText(hwnd, ID_ListaVacunaPrecio, auxVacunaPrecio);
					miVacuna = auxVacuna;
					EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaTipo), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaMarca), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaNo), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaDesc), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaPrecio), true);
					EnableWindow(GetDlgItem(hwnd, ID_bListaElimVacuna), true);
					EnableWindow(GetDlgItem(hwnd, ID_bListaModifVacuna), true);
				}
			}break;
			}
		}break;
		case ID_bListaModifVacuna: {
			vacuna* newVacunaInfo = new vacuna;
			GetDlgItemText(hwnd, ID_ListaVacunaTipo, newVacunaInfo->tipo, sizeof(newVacunaInfo->tipo));
			GetDlgItemText(hwnd, ID_ListaVacunaMarca, newVacunaInfo->marca, sizeof(newVacunaInfo->marca));
			GetDlgItemText(hwnd, ID_ListaVacunaNo, auxDosis2, sizeof(auxDosis2));
			newVacunaInfo->numDosis = atof(auxDosis2);
			GetDlgItemText(hwnd, ID_ListaVacunaClave, newVacunaInfo->claveVacuna, sizeof(newVacunaInfo->claveVacuna));
			GetDlgItemText(hwnd, ID_ListaVacunaDesc, newVacunaInfo->descripcion, sizeof(newVacunaInfo->descripcion));
			GetDlgItemText(hwnd, ID_ListaVacunaPrecio, auxVacunaPrecio2, sizeof(auxVacunaPrecio2));
			newVacunaInfo->precio = atof(auxVacunaPrecio2);

			if (newVacunaInfo->precio > 0) {
				ValidPrecio = true;
			}
			else {
				ValidPrecio = false;
			}

			auxVacuna = inicioVacuna;
			while (auxVacuna != nullptr) {
				if (strcmp(newVacunaInfo->claveVacuna, auxVacuna->claveVacuna) == 0) {
					if (strcmp(newVacunaInfo->claveVacuna, miVacuna->claveVacuna) == 0) {
						ValidClave = true;
					}
					else {
						ValidClave = false;
						break;
					}
				}
				else {
					ValidClave = true;
					auxVacuna = auxVacuna->sig;
				}
			}

			if (newVacunaInfo->numDosis >0 && newVacunaInfo->claveVacuna != "" && newVacunaInfo->descripcion != "") {
				if (ValidClave == true) {
					if (ValidPrecio) {
						modificarVacuna(newVacunaInfo, vacunaTextoList);
						delete newVacunaInfo;
						SetDlgItemText(hwnd, ID_ListaVacunaNo, "");
						SetDlgItemText(hwnd, ID_ListaVacunaClave, "");
						SetDlgItemText(hwnd, ID_ListaVacunaDesc, "");
						SetDlgItemText(hwnd, ID_ListaVacunaPrecio, "");
						EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaTipo), false);
						EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaMarca), false);
						EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaNo), false);
						EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaDesc), false);
						EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaPrecio), false);
						EnableWindow(GetDlgItem(hwnd, ID_bListaElimVacuna), false);
						EnableWindow(GetDlgItem(hwnd, ID_bListaModifVacuna), false);
						auxVacuna = inicioVacuna;
						while (auxVacuna != nullptr) {
							SendMessage(GetDlgItem(hwnd, ID_ListaListVacuna), LB_ADDSTRING, 0, (LPARAM)auxVacuna->claveVacuna);
							auxVacuna = auxVacuna->sig;
						}
					}
					else {
						MessageBox(hwnd, "El precio tiene que ser mayor que 0", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
				else {
					MessageBox(hwnd, "Es clave ya esta en uso", "AVISO", MB_OK | MB_ICONERROR);
				}
			}
			else {
				MessageBox(hwnd, "No pueden haber campos vacios", "AVISO", MB_OK | MB_ICONERROR);
			}
			
		}break;
		case ID_bListaElimVacuna: {
			int opc = MessageBox(hwnd, "¿Seguro que desea eliminar esta vacuna?", "AVISO", MB_OKCANCEL | MB_ICONQUESTION);
			if (opc == IDOK) {
				eliminarVacuna(vacunaTextoList);
				SetDlgItemText(hwnd, ID_ListaVacunaNo, "");
				SetDlgItemText(hwnd, ID_ListaVacunaClave, "");
				SetDlgItemText(hwnd, ID_ListaVacunaDesc, "");
				SetDlgItemText(hwnd, ID_ListaVacunaPrecio, "");
				EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaTipo), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaMarca), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaNo), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaDesc), false);
				EnableWindow(GetDlgItem(hwnd, ID_ListaVacunaPrecio), false);
				EnableWindow(GetDlgItem(hwnd, ID_bListaElimVacuna), false);
				EnableWindow(GetDlgItem(hwnd, ID_bListaModifVacuna), false);
				SendMessage(GetDlgItem(hwnd, ID_ListaListVacuna), LB_RESETCONTENT, 0, 0);
				auxVacuna = inicioVacuna;
				while (auxVacuna != nullptr) {
					SendMessage(GetDlgItem(hwnd, ID_ListaListVacuna), LB_ADDSTRING, 0, (LPARAM)auxVacuna->claveVacuna);
					auxVacuna = auxVacuna->sig;
				}
			}
		}break;
		case ID_bSalirListaVacuna:
			EndDialog(hwnd, 0);
		}break;
	}break;
	}
	return false;
}

LRESULT CALLBACK procVRegistrarCarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	SYSTEMTIME RegisFecha;
	char RegisDosis[5] = "asd";
	bool validDosis = false;
	int auxdosis = 0;
	switch (msg) {
	case WM_INITDIALOG: {
		auxPersona = inicioPersona;
		while (auxPersona != nullptr) {
			SendMessage(GetDlgItem(hwnd, ID_RegisCarnetCurp), LB_ADDSTRING, 0, (LPARAM)auxPersona->curp);
			auxPersona = auxPersona->sig;
		}
		auxVacuna = inicioVacuna;
		while (auxVacuna != nullptr) {
			SendMessage(GetDlgItem(hwnd, ID_RegisCarnetClave), LB_ADDSTRING, 0, (LPARAM)auxVacuna->claveVacuna);
			auxVacuna = auxVacuna->sig;
		}
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_RegisCarnetCurp: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				countCarnet1 = SendDlgItemMessage(hwnd, ID_RegisCarnetCurp, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, ID_RegisCarnetCurp, LB_GETTEXT, countCarnet1, (LPARAM)carnetCurpList);
				auxPersona = inicioPersona;
				while (auxPersona != nullptr && strcmp(carnetCurpList, auxPersona->curp) != 0) {
					auxPersona = auxPersona->sig;
				}
				if (strcmp(carnetCurpList, auxPersona->curp) == 0) {
					selectCurp = true;
					if (selectCurp == true && selectVacuna == true) {
						EnableWindow(GetDlgItem(hwnd, ID_bRegisCarnetRegistrar), true);
					}
				}
			}break;
			}
		}break;
		case ID_RegisCarnetClave: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				countCarnet2 = SendDlgItemMessage(hwnd, ID_RegisCarnetClave, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, ID_RegisCarnetClave, LB_GETTEXT, countCarnet2, (LPARAM)carnetVacunaList);
				auxVacuna = inicioVacuna;
				while (auxVacuna != nullptr && strcmp(carnetVacunaList, auxVacuna->claveVacuna) != 0) {
					auxVacuna = auxVacuna->sig;
				}
				if (strcmp(carnetVacunaList, auxVacuna->claveVacuna) == 0) {
					selectVacuna = true;
					if (selectCurp == true && selectVacuna == true) {
						EnableWindow(GetDlgItem(hwnd, ID_bRegisCarnetRegistrar), true);
					}
				}
			}break;
			}
		}break;
		case ID_bRegisCarnetRegistrar: {
			carnet* newCarnet = new carnet;
			GetDlgItemText(hwnd, ID_RegisCarnetDosis, RegisDosis, sizeof(RegisDosis));
			newCarnet->dosis = atoi(RegisDosis);
			strcpy_s(newCarnet->vacuna, carnetVacunaList);
			strcpy_s(newCarnet->curp, carnetCurpList);
			SendDlgItemMessage(hwnd, ID_RegisCarnetFecha, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (RegisFecha));
			newCarnet->dia = RegisFecha.wDay;
			newCarnet->mes = RegisFecha.wMonth;
			newCarnet->año = RegisFecha.wYear;
			GetDlgItemText(hwnd, ID_RegisCarnetLote, newCarnet->lote, sizeof(newCarnet->lote));
			GetDlgItemText(hwnd, ID_RegisCarnetCentro, newCarnet->centroVacuna, sizeof(newCarnet->centroVacuna));
			strcpy_s(newCarnet->foto, auxPersona->foto);
			auxCarnet = inicioCarnet;
			if (inicioCarnet == nullptr) {
				newCarnet->ID = 0;
			}
			else {
				while (auxCarnet->sig != nullptr) {
					auxCarnet = auxCarnet->sig;
				}
				newCarnet->ID = auxCarnet->ID + 1;
			}

			auxVacuna = inicioVacuna;
			while (auxVacuna != nullptr && strcmp(auxVacuna->claveVacuna, carnetVacunaList) != 0) {
				auxVacuna = auxVacuna->sig;
			}
			auxdosis = auxVacuna->numDosis;
			if (newCarnet->dosis > 0 && newCarnet->dosis <= auxdosis) {
				validDosis = true;
			}
			else {
				validDosis = false;
			}

			if (newCarnet->lote != "" && newCarnet->centroVacuna != "") {
				if (validDosis == true) {
					agregarCarnet(newCarnet);
					delete newCarnet;
					SendMessage(GetDlgItem(hwnd, ID_RegisCarnetCurp), LB_RESETCONTENT, 0, 0);
					SendMessage(GetDlgItem(hwnd, ID_RegisCarnetClave), LB_RESETCONTENT, 0, 0);
					auxPersona = inicioPersona;
					while (auxPersona != nullptr) {
						SendMessage(GetDlgItem(hwnd, ID_RegisCarnetCurp), LB_ADDSTRING, 0, (LPARAM)auxPersona->curp);
						auxPersona = auxPersona->sig;
					}
					auxVacuna = inicioVacuna;
					while (auxVacuna != nullptr) {
						SendMessage(GetDlgItem(hwnd, ID_RegisCarnetClave), LB_ADDSTRING, 0, (LPARAM)auxVacuna->claveVacuna);
						auxVacuna = auxVacuna->sig;
					}
					SetDlgItemText(hwnd, ID_RegisCarnetDosis, "");
					SetDlgItemText(hwnd, ID_RegisCarnetLote, "");
					SetDlgItemText(hwnd, ID_RegisCarnetCentro, "");
				}
				else {
					MessageBox(hwnd, "Esa dosis no es valida", "AVISO", MB_OK | MB_ICONERROR);
				}
			}
			else {
				MessageBox(hwnd, "No pueden haber campos vacios", "AVISO", MB_OK | MB_ICONERROR);
			}
			
		}break;
		case ID_bRegisCarnetSalir: {
			EndDialog(hwnd, 0);
			selectCurp = false;
			selectVacuna = false;
		}break;
		}
	}break;
	}
	return false;
}

LRESULT CALLBACK procVListaCarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char auxiliarDosis[5] = "asd";
	int auxdosisNum = 0;
	char auxClave[50] = "asd";
	SYSTEMTIME listaCarnet;
	SYSTEMTIME newFecha;
	newFecha.wMilliseconds = 0;
	newFecha.wSecond = 0;
	newFecha.wMinute = 0;
	newFecha.wHour = 0;
	listaCarnet.wMilliseconds = 0;
	listaCarnet.wSecond = 0;
	listaCarnet.wMinute = 0;
	listaCarnet.wHour = 0;
	bool encontroCurp = false, validDosis = false;
	char auxBusquedaSec[50] = "asd", auxDosis[5] = "asd";
	char auxBusquedaBin[50] = "asd";
	float auxBinario = 0;
	int auxBinario2 = 0, auxBinario3 = 0;
	switch (msg) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_bListaCarnetBusSec: {
			SendMessage(GetDlgItem(hwnd, ID_ListaCarnetCurp), LB_RESETCONTENT, 0, 0);
			GetDlgItemText(hwnd, ID_ListaCarnetBuscar, auxBusquedaSec, sizeof(auxBusquedaSec));
			auxCarnet = inicioCarnet;
			while (auxCarnet != nullptr) {
				if (strcmp(auxBusquedaSec, auxCarnet->curp) == 0) {
					SendMessage(GetDlgItem(hwnd, ID_ListaCarnetCurp), LB_ADDSTRING, 0, (LPARAM)auxCarnet->curp);
					encontroCurp = true;
				}
				auxCarnet = auxCarnet->sig;
			}
			if (encontroCurp == false) {
				MessageBox(hwnd, "No se encontro el Carnet", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_bListaCarnetBusBin: {
			busquedaBin = false;
			SendMessage(GetDlgItem(hwnd, ID_ListaCarnetCurp), LB_RESETCONTENT, 0, 0);
			GetDlgItemText(hwnd, ID_ListaCarnetBuscar, auxBusquedaBin, sizeof(auxBusquedaBin));
			auxBinario = atof(auxBusquedaBin);
			auxBinario2 = auxBinario;
			busquedaBinaria(inicioCarnet, nullptr, auxBinario2);
			auxCarnet = inicioCarnet;
			if (busquedaBin == true) {
				SendMessage(GetDlgItem(hwnd, ID_ListaCarnetCurp), LB_ADDSTRING, 0, (LPARAM)medio->curp);
			}
			else {
				MessageBox(hwnd, "No se encontro el Carnet", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_ListaCarnetCurp: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				countCarnet3 = SendDlgItemMessage(hwnd, ID_ListaCarnetCurp, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, ID_ListaCarnetCurp, LB_GETTEXT, countCarnet3, (LPARAM)carnetListaCurp);
				auxCarnet = inicioCarnet;
				while (auxCarnet != nullptr && strcmp(carnetListaCurp, auxCarnet->curp) != 0) {
					auxCarnet = auxCarnet->sig;
				}
				if (strcmp(carnetListaCurp, auxCarnet->curp) == 0) {
					SetDlgItemText(hwnd, ID_ListaCarnetVacuna, auxCarnet->vacuna);
					_itoa_s(auxCarnet->dosis, auxDosis, 5);
					SetDlgItemText(hwnd, ID_ListaCarnetDosis, auxDosis);
					listaCarnet.wDay = auxCarnet->dia;
					listaCarnet.wMonth = auxCarnet->mes;
					listaCarnet.wYear = auxCarnet->año;
					SendDlgItemMessage(hwnd, ID_ListaCarnetFecha, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&listaCarnet);
					SetDlgItemText(hwnd, ID_ListaCarnetLote, auxCarnet->lote);
					SetDlgItemText(hwnd, ID_ListaCarnetCentro, auxCarnet->centroVacuna);
					HBITMAP bmp;
					bmp = (HBITMAP)LoadImage(NULL, auxCarnet->foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					SendDlgItemMessage(hwnd, ID_ListaCarnetFoto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
					strcpy_s(zFile, auxCarnet->foto);
					EnableWindow(GetDlgItem(hwnd, ID_ListaCarnetDosis), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaCarnetFecha), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaCarnetLote), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaCarnetCentro), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaCarnetElim), true);
					EnableWindow(GetDlgItem(hwnd, ID_ListaCarnetModif), true);
					EnableWindow(GetDlgItem(hwnd, ID_bListaCarnetBuscar), true);
				}		
			}break;
			}
		}break;
		case ID_bListaCarnetBuscar: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp;

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendDlgItemMessage(hwnd, ID_ListaCarnetFoto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}
			else {
				MessageBox(hwnd, "No seleccionó foto", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_ListaCarnetModif: {
			carnet* newInfoCarnet = new carnet;
			strcpy_s(newInfoCarnet->curp, carnetListaCurp);
			GetDlgItemText(hwnd, ID_ListaCarnetDosis, auxiliarDosis, sizeof(auxiliarDosis));
			GetDlgItemText(hwnd, ID_ListaCarnetVacuna, auxClave, sizeof(auxClave));
			newInfoCarnet->dosis = atoi(auxiliarDosis);
			auxVacuna = inicioVacuna;
			while (auxVacuna != nullptr && strcmp(auxClave, auxVacuna->claveVacuna) != 0) {
				auxVacuna = auxVacuna->sig;
			}
			if (newInfoCarnet->dosis > 0 && newInfoCarnet->dosis <= auxVacuna->numDosis) {
				validDosis = true;
			}
			else {
				validDosis = false;
			}
			GetDlgItemText(hwnd, ID_ListaCarnetLote, newInfoCarnet->lote, sizeof(newInfoCarnet->lote));
			GetDlgItemText(hwnd, ID_ListaCarnetCentro, newInfoCarnet->centroVacuna, sizeof(newInfoCarnet->centroVacuna));
			SendDlgItemMessage(hwnd, ID_ListaCarnetFecha, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (newFecha));
			newInfoCarnet->dia = newFecha.wDay;
			newInfoCarnet->mes = newFecha.wMonth;
			newInfoCarnet->año = newFecha.wYear;
			strcpy_s(newInfoCarnet->foto, zFile);
			if (newInfoCarnet->lote != "" && newInfoCarnet->centroVacuna != "") {
				if (validDosis == true) {
					modificarCarnet(newInfoCarnet, carnetListaCurp);
					delete newInfoCarnet;
					EnableWindow(GetDlgItem(hwnd, ID_ListaCarnetDosis), false);
					EnableWindow(GetDlgItem(hwnd, ID_ListaCarnetFecha), false);
					EnableWindow(GetDlgItem(hwnd, ID_ListaCarnetLote), false);
					EnableWindow(GetDlgItem(hwnd, ID_ListaCarnetCentro), false);
					EnableWindow(GetDlgItem(hwnd, ID_ListaCarnetElim), false);
					EnableWindow(GetDlgItem(hwnd, ID_ListaCarnetModif), false);
					EnableWindow(GetDlgItem(hwnd, ID_bListaCarnetBuscar), false);
				}
				else {
					MessageBox(hwnd, "Esa dosis no es valida", "AVISO", MB_OK | MB_ICONERROR);
				}
			}
			else {
				MessageBox(hwnd, "No pueden haber campos vacios", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_ListaCarnetElim: {
			int opc = MessageBox(hwnd, "¿Seguro que desea eliminar esta carnet?", "AVISO", MB_OKCANCEL | MB_ICONQUESTION);
			if (opc == IDOK) {
				eliminarCarnet(carnetListaCurp);
				SetDlgItemText(hwnd, ID_ListaCarnetVacuna, "");
				SetDlgItemText(hwnd, ID_ListaCarnetDosis, "");
				SetDlgItemText(hwnd, ID_ListaCarnetLote, "");
				SetDlgItemText(hwnd, ID_ListaCarnetCentro, "");
				SetDlgItemText(hwnd, ID_ListaCarnetFoto, "");
				SetDlgItemText(hwnd, ID_ListaCarnetBuscar, "");
				SendMessage(GetDlgItem(hwnd, ID_ListaCarnetCurp), LB_RESETCONTENT, 0, 0);
			}
		}break;
		case ID_bListaCarnetSalir: {
			EndDialog(hwnd, 0);
		}break;
		}
	}break;
	}
	return false;
}

void agregarUsuario(usuario* nuevo) {
	if (inicioUsuario == nullptr) {
		inicioUsuario = new usuario;
		auxUsuario = inicioUsuario;
		auxUsuario->ant = nullptr;
		auxUsuario->sig = nullptr;

		strcpy_s(auxUsuario->clave, nuevo->clave);
		strcpy_s(auxUsuario->contra, nuevo->contra);
		strcpy_s(auxUsuario->ApllPat, nuevo->ApllPat);
		strcpy_s(auxUsuario->ApllMat, nuevo->ApllMat);
		strcpy_s(auxUsuario->nombres, nuevo->nombres);
	}
	else {
		auxUsuario = inicioUsuario;
		while (auxUsuario->sig != nullptr) {
			auxUsuario = auxUsuario->sig;
		}
		auxUsuario->sig = new usuario;
		auxUsuario->sig->sig = nullptr;
		auxUsuario->sig->ant = auxUsuario;
		auxUsuario = auxUsuario->sig;

		strcpy_s(auxUsuario->clave, nuevo->clave);
		strcpy_s(auxUsuario->contra, nuevo->contra);
		strcpy_s(auxUsuario->ApllPat, nuevo->ApllPat);
		strcpy_s(auxUsuario->ApllMat, nuevo->ApllMat);
		strcpy_s(auxUsuario->nombres, nuevo->nombres);
	}
	MessageBox(0, "Usuario registrado", "AVISO", MB_OK);
}

void escribirUsuarioBin() {
	auxUsuario = inicioUsuario;
	ofstream escribirUsuarios;
	escribirUsuarios.open("C:\\Users\\mat2k\\source\\repos\\Estructura de Datos 1er Entrega\\Usuarios.bin", ios::out | ios::binary | ios::trunc);
	if (escribirUsuarios.is_open()) {
		while (auxUsuario != nullptr) {
			escribirUsuarios.write((char*)auxUsuario, sizeof(usuario));
			auxUsuario = auxUsuario->sig;
		}
		escribirUsuarios.close();
	}
	else {
		MessageBox(0, "No se pudo abrir el archivo", "AVISO", MB_OK);
	}
}

void leerUsuarioBin() {
	ifstream archivoUsuarios;
	archivoUsuarios.open("C:\\Users\\mat2k\\source\\repos\\Estructura de Datos 1er Entrega\\Usuarios.bin", ios::in | ios::binary);

	if (archivoUsuarios.is_open()) {
		usuario* usuarioLeido = new usuario;
		while (!archivoUsuarios.read((char*)usuarioLeido, sizeof(usuario)).eof()) {
			if (inicioUsuario == nullptr) {
				inicioUsuario = usuarioLeido;
				inicioUsuario->sig = nullptr;
				auxUsuario = inicioUsuario;
			}
			else {
				auxUsuario->sig = usuarioLeido;
				auxUsuario = auxUsuario->sig;
				auxUsuario->sig = nullptr;
			}
			usuarioLeido = new usuario;
		}
		archivoUsuarios.close();
		delete usuarioLeido;
	}
	else {
		MessageBox(0, "No se pudo abrir el archivo", "AVISO", MB_OK);
	}
}

void agregarPersona(persona* nuevo) {
	if (inicioPersona == nullptr) {
		inicioPersona = new persona;
		auxPersona = inicioPersona;
		auxPersona->ant = nullptr;
		auxPersona->sig = nullptr;

		strcpy_s(auxPersona->ApllPat, nuevo->ApllPat);
		strcpy_s(auxPersona->ApllMat, nuevo->ApllMat);
		strcpy_s(auxPersona->nombres, nuevo->nombres);
		strcpy_s(auxPersona->curp, nuevo->curp);
		strcpy_s(auxPersona->rfc, nuevo->rfc);
		auxPersona->dia = nuevo->dia;
		auxPersona->mes = nuevo->mes;
		auxPersona->año = nuevo->año;
		strcpy_s(auxPersona->calle, nuevo->calle);
		strcpy_s(auxPersona->colonia, nuevo->colonia);
		strcpy_s(auxPersona->ciudad, nuevo->ciudad);
		strcpy_s(auxPersona->estado, nuevo->estado);
		strcpy_s(auxPersona->estadoCivil, nuevo->estadoCivil);
		strcpy_s(auxPersona->telefono, nuevo->telefono);
		strcpy_s(auxPersona->docIdentidad, nuevo->docIdentidad);
		strcpy_s(auxPersona->sexo, nuevo->sexo);
		strcpy_s(auxPersona->gpoOcupacional, nuevo->gpoOcupacional);
		strcpy_s(auxPersona->perfilRiesgo, nuevo->perfilRiesgo);
		strcpy_s(auxPersona->foto, nuevo->foto);
	}
	else {
		auxPersona = inicioPersona;
		while (auxPersona->sig != nullptr) {
			auxPersona = auxPersona->sig;
		}
		auxPersona->sig = new persona;
		auxPersona->sig->sig = nullptr;
		auxPersona->sig->ant = auxPersona;
		auxPersona = auxPersona->sig;

		strcpy_s(auxPersona->ApllPat, nuevo->ApllPat);
		strcpy_s(auxPersona->ApllMat, nuevo->ApllMat);
		strcpy_s(auxPersona->nombres, nuevo->nombres);
		strcpy_s(auxPersona->curp, nuevo->curp);
		strcpy_s(auxPersona->rfc, nuevo->rfc);
		auxPersona->dia = nuevo->dia;
		auxPersona->mes = nuevo->mes;
		auxPersona->año = nuevo->año;
		strcpy_s(auxPersona->calle, nuevo->calle);
		strcpy_s(auxPersona->colonia, nuevo->colonia);
		strcpy_s(auxPersona->ciudad, nuevo->ciudad);
		strcpy_s(auxPersona->estado, nuevo->estado);
		strcpy_s(auxPersona->estadoCivil, nuevo->estadoCivil);
		strcpy_s(auxPersona->telefono, nuevo->telefono);
		strcpy_s(auxPersona->docIdentidad, nuevo->docIdentidad);
		strcpy_s(auxPersona->sexo, nuevo->sexo);
		strcpy_s(auxPersona->gpoOcupacional, nuevo->gpoOcupacional);
		strcpy_s(auxPersona->perfilRiesgo, nuevo->perfilRiesgo);
		strcpy_s(auxPersona->foto, nuevo->foto);
	}
	MessageBox(0, "Persona registrada", "AVISO", MB_OK);
}

void modificarPersona(persona* newInfo, char curpPersona[25]) {
	auxPersona = inicioPersona;
	if (auxPersona== nullptr) {
		MessageBox(0, "No hay personas registradas", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (auxPersona != nullptr && strcmp(auxPersona->curp, curpPersona) != 0) {
			auxPersona = auxPersona->sig;
		}
		if (auxPersona == nullptr) {
			MessageBox(0, "Persona no encontrada", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {
			strcpy_s(auxPersona->ApllPat, newInfo->ApllPat);
			strcpy_s(auxPersona->ApllMat, newInfo->ApllMat);
			strcpy_s(auxPersona->nombres, newInfo->nombres);
			strcpy_s(auxPersona->curp, newInfo->curp);
			strcpy_s(auxPersona->rfc, newInfo->curp);
			auxPersona->dia = newInfo->dia;
			auxPersona->mes = newInfo->mes;
			auxPersona->año = newInfo->año;
			strcpy_s(auxPersona->calle, newInfo->calle);
			strcpy_s(auxPersona->colonia, newInfo->colonia);
			strcpy_s(auxPersona->ciudad, newInfo->ciudad);
			strcpy_s(auxPersona->estado, newInfo->estado);
			strcpy_s(auxPersona->estadoCivil, newInfo->estadoCivil);
			strcpy_s(auxPersona->telefono, newInfo->telefono);
			strcpy_s(auxPersona->sexo, newInfo->sexo);
			strcpy_s(auxPersona->gpoOcupacional, newInfo->gpoOcupacional);
			strcpy_s(auxPersona->rfc, newInfo->rfc);
			strcpy_s(auxPersona->foto, newInfo->foto);
			strcpy_s(auxPersona->docIdentidad, newInfo->docIdentidad);
			MessageBox(0, "Persona modificada", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
	}
}

void eliminarPersona(char curpPersona[25]) {
	auxPersona = inicioPersona;
	if (auxPersona == nullptr) {
		MessageBox(0, "No hay personas registradas", "AVISO", MB_OK);
	}
	else {
		while (auxPersona != nullptr && strcmp(auxPersona->curp, curpPersona) != 0) {
			auxPersona = auxPersona->sig;
		}
		if (auxPersona == nullptr) {
			MessageBox(0, "No se encontro la persona", "AVISO", MB_OK);
		}
		else if (auxPersona == inicioPersona) {
			if (inicioPersona->sig = nullptr) {
				inicioPersona = nullptr;
				delete auxPersona;
				auxPersona = inicioPersona;
			}
			else {
				inicioPersona = inicioPersona->sig;
				auxPersona->sig->ant = nullptr;
				delete auxPersona;
				auxPersona = inicioPersona;
			}
			MessageBox(0, "Persona eliminada", "AVISO", MB_OK);
		}
		else {
			if (auxPersona->sig == nullptr) {
				auxPersona->ant->sig = nullptr;
				delete auxPersona;
				auxPersona = inicioPersona;
			}
			else {
				auxPersona->sig->ant = auxPersona->ant;
				auxPersona->ant->sig = auxPersona->sig;
				delete auxPersona;
				auxPersona = inicioPersona;
			}
			MessageBox(0, "Persona eliminada", "AVISO", MB_OK);
		}
	}
}

void escribirPersonaBin() {
	auxPersona = inicioPersona;
	ofstream escribirPersonas;
	escribirPersonas.open("C:\\Users\\mat2k\\source\\repos\\Estructura de Datos 1er Entrega\\Personas.bin", ios::out | ios::binary | ios::trunc);
	if (escribirPersonas.is_open()) {
		while (auxPersona != nullptr) {
			escribirPersonas.write((char*)auxPersona, sizeof(persona));
			auxPersona = auxPersona->sig;
		}
		escribirPersonas.close();
	}
	else {
		MessageBox(0, "No se pudo abrir el archivo", "AVISO", MB_OK);
	}
}

void leerPersonaBin() {
	ifstream archivoPersonas;
	archivoPersonas.open("C:\\Users\\mat2k\\source\\repos\\Estructura de Datos 1er Entrega\\Personas.bin", ios::in | ios::binary);

	if (archivoPersonas.is_open()) {
		persona* personaLeida = new persona;
		while (!archivoPersonas.read((char*)personaLeida, sizeof(persona)).eof()) {
			if (inicioPersona == nullptr) {
				inicioPersona = personaLeida;
				inicioPersona->sig = nullptr;
				auxPersona = inicioPersona;
			}
			else {
				auxPersona->sig = personaLeida;
				auxPersona = auxPersona->sig;
				auxPersona->sig = nullptr;
			}
			personaLeida = new persona;
		}
		archivoPersonas.close();
		delete personaLeida;
	}
	else {
		MessageBox(0, "No se pudo abrir el archivo", "AVISO", MB_OK);
	}
}

void agregarVacuna(vacuna* nuevo) {
	if (inicioVacuna == nullptr) {
		inicioVacuna = new vacuna;
		auxVacuna = inicioVacuna;
		auxVacuna->ant = nullptr;
		auxVacuna->sig = nullptr;

		strcpy_s(auxVacuna->tipo, nuevo->tipo);
		strcpy_s(auxVacuna->marca, nuevo->marca);
		auxVacuna->numDosis = nuevo->numDosis;
		strcpy_s(auxVacuna->claveVacuna, nuevo->claveVacuna);
		strcpy_s(auxVacuna->descripcion, nuevo->descripcion);
		auxVacuna->precio = nuevo->precio;
	}
	else{
		auxVacuna = inicioVacuna;
		while (auxVacuna->sig != nullptr) {
			auxVacuna = auxVacuna->sig;
		}
		auxVacuna->sig = new vacuna;
		auxVacuna->sig->sig = nullptr;
		auxVacuna->sig->ant = auxVacuna;
		auxVacuna = auxVacuna->sig;

		strcpy_s(auxVacuna->tipo, nuevo->tipo);
		strcpy_s(auxVacuna->marca, nuevo->marca);
		auxVacuna->numDosis = nuevo->numDosis;
		strcpy_s(auxVacuna->claveVacuna, nuevo->claveVacuna);
		strcpy_s(auxVacuna->descripcion, nuevo->descripcion);
		auxVacuna->precio = nuevo->precio;
	}
	MessageBox(0, "Vacuna registrada", "AVISO", MB_OK);
}

void modificarVacuna(vacuna* newInfo, char claveVacuna[20]) {
	auxVacuna = inicioVacuna;
	if (auxVacuna == nullptr) {
		MessageBox(0, "No hay vacunas registradas", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (auxVacuna != nullptr && strcmp(auxVacuna->claveVacuna, claveVacuna) != 0) {
			auxVacuna = auxVacuna->sig;
		}
		if (auxVacuna == nullptr) {
			MessageBox(0, "Vacuna no encontrada", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {
			strcpy_s(auxVacuna->tipo, newInfo->tipo);
			strcpy_s(auxVacuna->marca, newInfo->marca);
			auxVacuna->numDosis = newInfo->numDosis;
			strcpy_s(auxVacuna->claveVacuna, newInfo->claveVacuna);
			strcpy_s(auxVacuna->descripcion, newInfo->descripcion);
			auxVacuna->precio = newInfo->precio;
			MessageBox(0, "Vacuna modificada", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
	}
}

void eliminarVacuna(char claveVacuna[20]) {
	auxVacuna = inicioVacuna;
	if (auxVacuna == nullptr) {
		MessageBox(0, "No hay vacunas registradas", "AVISO", MB_OK);
	}
	else {
		while (auxVacuna != nullptr && strcmp(auxVacuna->claveVacuna, claveVacuna) != 0) {
			auxVacuna = auxVacuna->sig;
		}
		if (auxVacuna == nullptr) {
			MessageBox(0, "No se encontro la vacuna", "AVISO", MB_OK);
		}
		else if (auxVacuna == inicioVacuna) {
			if (auxVacuna->sig = nullptr) {
				inicioVacuna = nullptr;
				delete auxVacuna;
				auxVacuna = inicioVacuna;
			}
			else {
				inicioVacuna = inicioVacuna->sig;
				auxVacuna->sig->ant = nullptr;
				delete auxVacuna;
				auxVacuna = inicioVacuna;
			}
			MessageBox(0, "Persona eliminada", "AVISO", MB_OK);
		}
		else {
			if (auxVacuna->sig == nullptr) {
				auxVacuna->ant->sig = nullptr;
				delete auxVacuna;
				auxVacuna = inicioVacuna;
			}
			else {
				auxVacuna->sig->ant = auxVacuna->ant;
				auxVacuna->ant->sig = auxVacuna->sig;
				delete auxVacuna;
				auxVacuna = inicioVacuna;
			}
			MessageBox(0, "Persona eliminada", "AVISO", MB_OK);
		}
	}
}

void escribirVacunaBin() {
	auxVacuna = inicioVacuna;
	ofstream escribirVacuna;
	escribirVacuna.open("C:\\Users\\mat2k\\source\\repos\\Estructura de Datos 1er Entrega\\Vacunas.bin", ios::out | ios::binary | ios::trunc);
	if (escribirVacuna.is_open()) {
		while (auxVacuna != nullptr) {
			escribirVacuna.write((char*)auxVacuna, sizeof(vacuna));
			auxVacuna = auxVacuna->sig;
		}
		escribirVacuna.close();
	}
	else {
		MessageBox(0, "No se pudo abrir el archivo", "AVISO", MB_OK);
	}
}

void leerVacunasBin() {
	ifstream archivoVacunas;
	archivoVacunas.open("C:\\Users\\mat2k\\source\\repos\\Estructura de Datos 1er Entrega\\Vacunas.bin", ios::in | ios::binary);

	if (archivoVacunas.is_open()) {
		vacuna* vacunasLeido = new vacuna;
		while (!archivoVacunas.read((char*)vacunasLeido, sizeof(vacuna)).eof()) {
			if (inicioVacuna == nullptr) {
				inicioVacuna = vacunasLeido;
				inicioVacuna->sig = nullptr;
				auxVacuna = inicioVacuna;
			}
			else {
				auxVacuna->sig = vacunasLeido;
				auxVacuna = auxVacuna->sig;
				auxVacuna->sig = nullptr;
			}
			vacunasLeido = new vacuna;
		}
		archivoVacunas.close();
		delete vacunasLeido;
	}
	else {
		MessageBox(0, "No se pudo abrir el archivo", "AVISO", MB_OK);
	}
}

void agregarCarnet(carnet* nuevo) {
	if (inicioCarnet == nullptr) {
		inicioCarnet = new carnet;
		auxCarnet = inicioCarnet;
		auxCarnet->ant = nullptr;
		auxCarnet->sig = nullptr;

		strcpy_s(auxCarnet->curp, nuevo->curp);
		strcpy_s(auxCarnet->vacuna, nuevo->vacuna);
		auxCarnet->dosis = nuevo->dosis;
		auxCarnet->dia = nuevo->dia;
		auxCarnet->mes = nuevo->mes;
		auxCarnet->año = nuevo->año;
		strcpy_s(auxCarnet->lote, nuevo->lote);
		strcpy_s(auxCarnet->centroVacuna, nuevo->centroVacuna);
		strcpy_s(auxCarnet->foto, nuevo->foto);
		auxCarnet->ID = nuevo->ID;
	}
	else {
		auxCarnet = inicioCarnet;
		while (auxCarnet->sig != nullptr) {
			auxCarnet = auxCarnet->sig;
		}
		auxCarnet->sig = new carnet;
		auxCarnet->sig->sig = nullptr;
		auxCarnet->sig->ant = auxCarnet;
		auxCarnet = auxCarnet->sig;

		strcpy_s(auxCarnet->curp, nuevo->curp);
		strcpy_s(auxCarnet->vacuna, nuevo->vacuna);
		auxCarnet->dosis = nuevo->dosis;
		auxCarnet->dia = nuevo->dia;
		auxCarnet->mes = nuevo->mes;
		auxCarnet->año = nuevo->año;
		strcpy_s(auxCarnet->lote, nuevo->lote);
		strcpy_s(auxCarnet->centroVacuna, nuevo->centroVacuna);
		strcpy_s(auxCarnet->foto, nuevo->foto);
		auxCarnet->ID = nuevo->ID;
	}
	MessageBox(0, "Carnet registrado", "AVISO", MB_OK);
}

int busquedaBinaria(carnet* inicio, carnet* fin, int buscar) {
	if (inicio == fin) {
		MessageBox(0, "Carnet no encontrado", "AVISO", MB_OK);
		busquedaBin = false;
		return 0;
	}

	int i = 0;
	int mitad = 0;
	medio = inicio;

	while (medio != fin) {
		medio = medio->sig;
		i++;
	}
	mitad = i / 2;
	medio = inicio;
	for (int j = 0; j < mitad; j++) {
		medio = medio->sig;
	}

	if (medio->ID == buscar) {
		busquedaBin = true;
		return medio->ID;
	}
	if (medio->ID > buscar) {
		return busquedaBinaria(inicio, medio, buscar);
	}
	if (medio->ID < buscar) {
		return busquedaBinaria(medio, fin, buscar);
	}
}

void modificarCarnet(carnet* newInfo, char curp[25]) {
	auxCarnet = inicioCarnet;
	if (auxCarnet == nullptr) {
		MessageBox(0, "No hay carnets registrados", "AVISO", MB_OK);
	}
	else {
		while (auxCarnet != nullptr && strcmp(auxCarnet->curp, curp) != 0) {
			auxCarnet = auxCarnet->sig;
		}
		if (auxCarnet == nullptr) {
			MessageBox(0, "carnet no encontrado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {
			auxCarnet->dosis = newInfo->dosis;
			strcpy_s(auxCarnet->lote,newInfo->lote);
			strcpy_s(auxCarnet->centroVacuna, newInfo->centroVacuna);
			auxCarnet->dia = newInfo->dia;
			auxCarnet->mes = newInfo->mes;
			auxCarnet->año = newInfo->año;
			strcpy_s(auxCarnet->foto, newInfo->foto);
			MessageBox(0, "Carnet modificado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
	}
}

void eliminarCarnet(char curp[25]) {
	auxCarnet = inicioCarnet;
	if (auxCarnet == nullptr) {
		MessageBox(0, "No hay carnets registrados", "AVISO", MB_OK);
	}
	else {
		while (auxCarnet != nullptr && strcmp(auxCarnet->curp, curp) != 0) {
			auxCarnet = auxCarnet->sig;
		}
		if (auxCarnet == nullptr) {
			MessageBox(0, "No se encontro el carnet", "AVISO", MB_OK);
		}
		else if (auxCarnet == inicioCarnet) {
			if (auxCarnet->sig = nullptr) {
				inicioCarnet = nullptr;
				delete auxCarnet;
				auxCarnet = inicioCarnet;
			}
			else {
				inicioCarnet = inicioCarnet->sig;
				inicioCarnet->ant = nullptr;
				delete auxCarnet;
				auxCarnet = inicioCarnet;
				while (auxCarnet != nullptr) {
					if (auxCarnet->sig == nullptr) {
						auxCarnet->ID--;
						break;
					}
					else{
						auxCarnet->ID--;
						auxCarnet = auxCarnet->sig;
					}
				}
			}
			MessageBox(0, "Persona eliminada", "AVISO", MB_OK);
		}
		else {
			if (auxCarnet->sig == nullptr) {
				auxCarnet->ant->sig = nullptr;
				delete auxCarnet;
				auxCarnet = inicioCarnet;
			}
			else {
				auxCarnet2 = auxCarnet;
				while (auxCarnet2 != nullptr) {
					if (auxCarnet2->sig == nullptr) {
						auxCarnet2->ID--;
						break;
					}
					else {
						auxCarnet2->ID--;
						auxCarnet2 = auxCarnet2->sig;
					}
				}
				auxCarnet->sig->ant = auxCarnet->ant;
				auxCarnet->ant->sig = auxCarnet->sig;
				delete auxCarnet;
				auxCarnet = inicioCarnet;
				auxCarnet2 = inicioCarnet;
			}
			MessageBox(0, "Persona eliminada", "AVISO", MB_OK);
		}
	}
}

void escribirCarnetBin() {
	auxCarnet = inicioCarnet;
	ofstream escribirCarnet;
	escribirCarnet.open("C:\\Users\\mat2k\\source\\repos\\Estructura de Datos 1er Entrega\\Carnet.bin", ios::out | ios::binary | ios::trunc);
	if (escribirCarnet.is_open()) {
		while (auxCarnet != nullptr) {
			escribirCarnet.write((char*)auxCarnet, sizeof(carnet));
			auxCarnet = auxCarnet->sig;
		}
		escribirCarnet.close();
	}
	else {
		MessageBox(0, "No se pudo abrir el archivo", "AVISO", MB_OK);
	}
}

void leerCarnetBin() {
	ifstream archivoCarnet;
	archivoCarnet.open("C:\\Users\\mat2k\\source\\repos\\Estructura de Datos 1er Entrega\\Carnet.bin", ios::in | ios::binary);

	if (archivoCarnet.is_open()) {
		carnet* carnetLeido = new carnet;
		while (!archivoCarnet.read((char*)carnetLeido, sizeof(carnet)).eof()) {
			if (inicioCarnet == nullptr) {
				inicioCarnet = carnetLeido;
				inicioCarnet->sig = nullptr;
				auxCarnet = inicioCarnet;
			}
			else {
				auxCarnet->sig = carnetLeido;
				auxCarnet = auxCarnet->sig;
				auxCarnet->sig = nullptr;
			}
			carnetLeido = new carnet;
		}
		archivoCarnet.close();
		delete carnetLeido;
	}
	else {
		MessageBox(0, "No se pudo abrir el archivo", "AVISO", MB_OK);
	}
}