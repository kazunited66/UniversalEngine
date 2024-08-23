#include "Game/GameObjects/UWorldObject.h"
#include "Graphics/UGraphicsEngine.h"
#include "Graphics/UModel.h"

TWeak<UModel> UWorldObject::ImportModel(const UString& path)
{
    if (const auto& modelRef = UGameEngine::GetGameEngine()->GetGraphics()->ImportModel(path)) {
        m_objectModels.push_back(modelRef);
        modelRef->GetTransform() = m_transform;
        return modelRef; 
    }
    return {};
}

TWeak<USCollision> UWorldObject::AddCollision(const USBox& box, const bool& debug)
{
    //create the collision 
    const TShared<USCollision>& newCol = TMakeShared<USCollision>(); 
    //set the position and size 
    newCol->box = box;
   
    //TO DO: DEBUG COLLISION 
    m_objectCollisions.push_back(newCol);

    //return a weak version 
    return newCol;
}

void UWorldObject::TestCollision(const TShared<UWorldObject>& other)
{
    //looping throught this object c ollisions 
    for (const auto& col : m_objectCollisions) {
        //loop through the other objects collisions 
        for (const auto& otherCol : other->m_objectCollisions) {
            if (USCollision::IsOverlapping(*col, *otherCol)) {
                OnOverlap(other,otherCol); 
            }
        }
    }
}

void UWorldObject::OnPostTick(float deltaTime)
{
    UObject::OnPostTick(deltaTime); 

    //all models will follow the world object 
    for (const auto& modelRef : m_objectModels) {
        modelRef->GetTransform() = GetTransform() ; 
    }

    //all collisions will follow the world object 
    for (const auto& colRef : m_objectCollisions) {
        colRef->box.position  = GetTransform().position;
    }
}

