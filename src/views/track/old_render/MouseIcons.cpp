/*
 * Copyright 2013, Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include "MouseIcons.h"

BCursor* MouseIcons::fArrow = NULL;
BCursor* MouseIcons::fArrowLeft = NULL;
BCursor* MouseIcons::fArrowRight = NULL;
BCursor* MouseIcons::fPencil = NULL;
BCursor* MouseIcons::fMove = NULL;
BCursor* MouseIcons::fMoveArrow = NULL;
BCursor* MouseIcons::fLeftRight = NULL;


BCursor*
MouseIcons::MouseArrow()
{
	if (fArrow == NULL)
		fArrow = new BCursor(IMouse_Arrow);

	return fArrow;
}


BCursor*
MouseIcons::MouseArrowLeft()
{
	if (fArrowLeft == NULL)
		fArrowLeft = new BCursor(IMouse_ArrowLeft);

	return fArrowLeft;
}


BCursor*
MouseIcons::MouseArrowRight()
{
	if (fArrowRight == NULL)
		fArrowRight = new BCursor(IMouse_ArrowRight);

	return fArrowRight;
}


BCursor*
MouseIcons::MousePencil()
{
	if (fPencil == NULL)
		fPencil = new BCursor(IMouse_Pencil);

	return fPencil;
}


BCursor*
MouseIcons::MouseMove()
{
	if (fMove == NULL)
		fMove = new BCursor(IMouse_Move);

	return fMove;
}


BCursor*
MouseIcons::MouseArrowMove() 
{
	if (fMoveArrow == NULL)
		fMoveArrow = new BCursor(IMouse_MoveArrow);

	return fMoveArrow;
}


BCursor*
MouseIcons::MouseLeftRight()
{ 
	if (fLeftRight == NULL)
 		fLeftRight = new BCursor(IMouse_LeftRight);

	return fLeftRight;
}
