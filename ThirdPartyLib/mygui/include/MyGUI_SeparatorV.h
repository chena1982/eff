/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_SEPARATORV_H__
#define __MYGUI_SEPARATORV_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	/** \brief @wpage{Button}
		Button widget description should be here.
	*/
	class MYGUI_EXPORT SeparatorV :
		public Widget,
		public MemberObsolete<SeparatorV>
	{
		MYGUI_RTTI_DERIVED( SeparatorV )

		friend class Widget;
	public:
		SeparatorV();
		virtual ~SeparatorV();
	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		void assignLeftRightPanel();
	private:
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
	private:
		MyGUI::Widget* mLeftPanel;
		MyGUI::Widget* mRightPanel;

		int mMinSizeLeft;
		int mMinSizeRight;

		MyGUI::IntCoord mStartLeftPanel;
		MyGUI::IntCoord mStartRightPanel;
		MyGUI::IntCoord mStartSeparatorV;

		MyGUI::IntPoint mStartMousePosition;
	};

} // namespace MyGUI

#endif // __MYGUI_SEPARATORV_H__
