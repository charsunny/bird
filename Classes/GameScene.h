//
//  GameScene.h
//  bricker
//
//  Created by Sun Xi on 7/26/14.
//
//

#ifndef __bricker__GameScene__
#define __bricker__GameScene__

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnter() override;
    
    void initStartUp();
    
    void initPipes();
    
    virtual void update(float dt) override;
    
    void updatePipePos();
    
    void updateBirdPos(float dt);
    
    void addPipeSprite(std::string pipename,  float posX);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
private:
    bool gameStart{false};
    constexpr static float fallSpeed = 1500;
    constexpr static float upDistance = 80;
    float birdSpeed{0};
    cocos2d::Sprite* landSpriteL;
    cocos2d::Sprite* landSpriteR;
    cocos2d::Sprite* birdSprite;
    cocos2d::Sprite* title;
    cocos2d::Sprite* tutorial;
    cocos2d::Label*  scoreLabel;
};


#endif /* defined(__bricker__GameScene__) */
