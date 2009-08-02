
import EFFBase
import EFF3D



	
	
class DeferredRenderingMaterial(EFF3D.EFF3DMaterial):
	def __init__(self):
		EFF3D.EFF3DMaterial.__init__(self)
		print "call drm Constructor"
		
	def test(self,material):
		print "call DeferredRenderingMaterial member function test."

def mytest(pMaterial):
	print "hehe."


drm = DeferredRenderingMaterial()

def OnMaterialBeforeRender(material):
	print "call OnMaterialBeforeRender."

def InitMaterialScript(material):
	print "call InitMaterialScript."
	material.OnMaterialBeforeRender.AddScriptMemberEventCall(drm,"test")
	print "end call InitMaterialScript."	
