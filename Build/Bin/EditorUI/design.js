var menubar = {
	view:"menu",
    data:[
		{id:"1", value:"File", submenu:[
			"Load", 
			"Save"]}
	],
	on:{
		onMenuItemClick:function(id) {
			webix.message("Click: "+this.getMenuItem(id).value);
		}
	},

    type:{subsign:false, height:26}
};

var tabbar = {
	view:"tabbar",
	multiview:true,
	options: [
		{ value:"Scene", id:"sceneViewWrap", labelWidth:120 },
		{ value:"Game", id:"gameView" }
	], 
	height:26,
	tabOffset:0,
	optionWidth:80
};

var sceneToolbar = {
	view:"toolbar",
	id:"sceneToolbar",
	height:26,
	padding:0,
	cols: [
		{view:"select", width:160, align:"right", value:"Textured", options:["Textured", "Wireframe", "Textured Wire", "Render Paths", "Lightmap Resolution"]},
		{view:"select", width:160, align:"right", value:"RGB", options:["RGB", "Alpha", "Overdraw", "Mipmaps"]},
	]
};

var sceneView = {
	id:"sceneViewWrap",
	rows:[
		sceneToolbar,
		{id:"sceneView"}
	]
};

var mainWindow = {
    view:"multiview",
    animate:false,
    cells:[
		sceneView,
		{id:"gameView"},
    ]
};


var mainFrame = {
	id:"layout",
	cols:[
		{rows:[
			tabbar,
			mainWindow,
			{view:"resizer"},
			{cols:[
				{id:"Hiereray", height:300, minHeight:300},
				{view:"resizer"},
				{id:"Project"}
			]}
		]},

		{view:"resizer", id:"resizer1"},

		{id:"inspector", width:300, minWidth:300}
	]
};


var ui_scheme = {
    rows:[menubar, mainFrame]
};