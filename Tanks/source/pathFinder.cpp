//
//Tank::Tank()
//{
//	Tank(glm::vec2(20,20), glm::vec2(0, 0));
//};
//
//Tank::Tank(glm::vec2 a_size, glm::vec2 a_position)
//{
//	mSpriteID = 0;
//	mPosition = a_position;
//	mSize = a_size;
//	mColor = glm::vec4(1, 1, 1, 1);
//	mMoveSpeed = 1;
//	mRotation = 0;
//}
//
//void Tank::FlipRotation()
//{
//	mRotation += 180;
//	if (mRotation >= 360) mRotation -= 360;
//}
//
//void Tank::Update(float deltaTime)
//{
//	if (pathList.size() > 0)
//	{
//		mGoalNode = pathList.front();
//		mGoalNode->mColor = glm::vec4(0, 0, 1, 1);
//		if (mCurrentLERPValue < 1)
//		{
//			mPosition = glm::lerp(mLastNodeVisited->mPosition, mGoalNode->mPosition, mCurrentLERPValue);
//			mCurrentLERPValue += mMoveSpeed * deltaTime;
//		}
//		else
//		{
//			mCurrentLERPValue = 0;
//			mLastNodeVisited = mGoalNode;
//			pathList.erase(pathList.begin());
//			if (pathList.size() > 0)
//				mGoalNode = pathList.front();
//		}
//	}
//	else
//	{
//		mGoalNode = nullptr;
//	}
//}