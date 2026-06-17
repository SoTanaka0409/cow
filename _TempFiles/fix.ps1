$content = [System.IO.File]::ReadAllText('Source/ResultScene.cpp', [System.Text.Encoding]::Default)
$content = $content -replace '(?s)ResultScene::ResultScene\(\).*?ResultScene::~ResultScene\(\)', 'ResultScene::ResultScene()
{
	mFlag = true;
	mnCount = 0;
	ResultGraphHandle = LoadGraph("Resource/リザルト.png");

	rankImage[0] = LoadGraph("Resource/1位.png");
	rankImage[1] = LoadGraph("Resource/2位.png");
	rankImage[2] = LoadGraph("Resource/3位.png");

	rankingTitleImage = LoadGraph("Resource/ランキング.png");

	yourScoreTextImg = LoadGraph("Resource/スコア.png");

	pointImg = LoadGraph("Resource/点.png");
	resultButton newGameBtn;
	newGameBtn.type = SelectionManager::Title::title;
	newGameBtn.graphHandle = LoadGraph("Resource/スタートボタン1.png");
	newGameBtn.x = 920;
	newGameBtn.y = 50;
	GetGraphSize(newGameBtn.graphHandle, &newGameBtn.w, &newGameBtn.h);
	newGameBtn.isHover = false;
	mButtons.push_back(newGameBtn);

	resultButton exitBtn;
	exitBtn.type = SelectionManager::Title::titleOUT;
	exitBtn.graphHandle = LoadGraph("Resource/しゅうりょうぼたん (1).png");
	exitBtn.x = 960;
	exitBtn.y = 680;
	GetGraphSize(exitBtn.graphHandle, &exitBtn.w, &exitBtn.h);
	exitBtn.isHover = false;
	mButtons.push_back(exitBtn);

	Master::mpSoundManager->PlayBGM(SoundManager::BGM_RESULT);
	Master::mpSoundManager->SetBGMVolume(120);
}

ResultScene::~ResultScene()'
[System.IO.File]::WriteAllText('Source/ResultScene.cpp', $content, [System.Text.Encoding]::UTF8)
