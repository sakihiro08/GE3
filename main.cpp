#include"Winapp.h"
#include"input.h"
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include"Sprite.h"
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{

#pragma region 基礎システムの初期化
    
    //ポインタ
    Winapp* winApp = nullptr;
    winApp = new Winapp();
    winApp->Initialize();
    Input* input = nullptr;
    HRESULT result;
    MSG msg{};  // メッセージ
#pragma endregion
     //入力の初期化
    Input* input = nullptr;
    input = new Input();
    input->Initialize(winApp);

#pragma endregion　基礎システムの初期化
    //ポインタ
    DirectXCommon* dxCommon = nullptr;
    dxCommon = new DirectXCommon();
    dxCommon->Initialize(winApp);
    // DirectInputの初期化
    SpriteCommon* spriteCommon = nullptr;
    spriteCommon = new SpriteCommon;
    spriteCommon->Initialize();




#pragma region 最初のシーンの初期化
   
    Sprite* sprite = new Sprite();
     sprite->Initialize(sptriteCommon);
    
#pragma endregion 最初のシーンの初期化

   
// ゲームループ
    while (true) {
#pragma region 基礎システムの更新
        // メッセージがある？
        if (winApp->ProcessMessage() == true)
        {
            break;
        }
#pragma endregion 基礎システムの更新
        input->Update();
#pragma region 基礎システムの更新
#pragma endregion 基礎システムの更新
#pragma region 最初のシーンの更新
        //ポケモンアニメ出禁を書く
        dxCommon->PreDraw();
        spriteCommon->PreDraw();
        sprite->Draw();
#pragma endregion 最初のシーンの更新
        dxCommon->PostDraw();
    }
#pragma region 最初のシーンの終了
   
    delete sprite;
    sprite = nullptr;
#pragma endregion 最初のシーンの終了
#pragma endregion 基礎システム終了
    delete spriteCommon;
    spriteCommon = nullptr;
     delete input; 
     input = nullptr;
     delete dxCommon;
     dxCommon = nullptr;
     winApp->Finalize();   
     delete winApp;
    return 0;
}
