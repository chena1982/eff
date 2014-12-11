/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_SEPARATORH_H__
#define __MYGUI_SEPARATORH_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	/** \brief @wpage{Button}
		Button widget description should be here.
	*/
	class MYGUI_EXPORT SeparatorH :
		public Widget,
		public MemberObsolete<SeparatorH>
	{
		MYGUI_RTTI_DERIVED( SeparatorH )

		friend class Widget;
	public:
		SeparatorH();
		virtual ~SeparatorH();
	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		void assignTopBottomPanel();
	private:
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
	private:
		MyGUI::Widget* mTopPanel;
		MyGUI::Widget* mBottomPanel;

		int mMinSizeTop;
		int mMinSizeBottom;

		MyGUI::IntCoord mStartTopPanel;
		MyGUI::IntCoord mStartBottomPanel;
		MyGUI::IntCoord mStartSeparatorH;

		MyGUI::IntPoint mStartMousePosition;
	};

	std::string FindParentPrefix(MyGUI::Widget* _parent);

} // namespace MyGUI

#endif // __MYGUI_SEPARATORH_H__
