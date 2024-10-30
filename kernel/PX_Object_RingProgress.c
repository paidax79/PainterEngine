#include "PX_Object_RingProgress.h"

PX_OBJECT_RENDER_FUNCTION(PX_Object_RingProgressRender)
{
	PX_Object_RingProgress* pDesc = PX_ObjectGetDesc(PX_Object_RingProgress, pObject);
	px_int angle;
	px_float objx, objy, objHeight, objWidth;
	px_float inheritX, inheritY;

	PX_ObjectGetInheritXY(pObject, &inheritX, &inheritY);

	objx = (pObject->x + inheritX);
	objy = (pObject->y + inheritY);
	objWidth = pObject->Width;
	objHeight = pObject->Height;

	
	if (pDesc->current_value < pDesc->target_value)
	{
		px_float inc = pDesc->Speed * elapsed / 1000;
		if (inc > pDesc->target_value - pDesc->current_value)
		{
			pDesc->current_value = pDesc->target_value;
		}
		else
		{
			pDesc->current_value += inc;
		}
	}
	else if (pDesc->current_value > pDesc->target_value)
	{
		px_float inc = pDesc->Speed * elapsed / 1000;
		if (inc > pDesc->current_value - pDesc->target_value)
		{
			pDesc->current_value = pDesc->target_value;
		}
		else
		{
			pDesc->current_value -= inc;
		}
	}

	

	PX_GeoDrawRing(psurface, (px_int)objx, (px_int)objy, pObject->diameter / 2, 6, pDesc->backgroundColor,0,360);
	PX_GeoDrawRing(psurface, (px_int)objx, (px_int)objy, pObject->diameter / 2, 7, pDesc->borderColor, 0, 360);


	angle = (px_int)((pDesc->current_value)/pDesc->max * 360);
	
	PX_GeoDrawRing(psurface, (px_int)objx, (px_int)objy, pObject->diameter / 2, 5, pDesc->processColor, -90, -90 + angle);


	do 
	{
		px_char content[32];
		if (pDesc->target_value-(px_int)pDesc->target_value)
		{
			PX_sprintf1(content, sizeof(content), "%1.2", PX_STRINGFORMAT_FLOAT(pDesc->current_value));
			PX_FontModuleDrawText(psurface, pDesc->numericFM, (px_int)objx, (px_int)objy, PX_ALIGN_CENTER, content, (pDesc->fontColor));
		}
		else
		{
			PX_itoa((px_int)pDesc->current_value, content, sizeof(content), 10);
			PX_FontModuleDrawText(psurface, pDesc->numericFM, (px_int)objx, (px_int)objy, PX_ALIGN_CENTER, content, (pDesc->fontColor));
		}
		
	} while (0);

}

PX_Object* PX_Object_RingProgressAttachObject( PX_Object* pObject,px_int attachIndex, px_float x, px_float y, px_float radius, PX_FontModule* fm)
{
	PX_Object_RingProgress Desc,*pdesc;

	PX_memset(&Desc, 0, sizeof(Desc));
	Desc.current_value = 0;
	Desc.max = 100;
	Desc.numericFM = fm;
	Desc.backgroundColor = PX_OBJECT_UI_DEFAULT_BACKGROUNDCOLOR;
	Desc.borderColor = PX_OBJECT_UI_DEFAULT_BORDERCOLOR;
	Desc.processColor = PX_COLOR(128, 255, 64, 64);
	Desc.fontColor = PX_OBJECT_UI_DEFAULT_FONTCOLOR;
	Desc.min = 0;
	Desc.target_value = 0;
	Desc.Speed = 100;
	Desc.target_value = 0;

	PX_ASSERTIF(pObject == PX_NULL);
	PX_ASSERTIF(attachIndex < 0 || attachIndex >= PX_COUNTOF(pObject->pObjectDesc));
	PX_ASSERTIF(pObject->pObjectDesc[attachIndex] != PX_NULL);
	pdesc = (PX_Object_RingProgress*)PX_ObjectCreateDesc(pObject, attachIndex, PX_OBJECT_TYPE_RINGPROGRESS, 0, PX_Object_RingProgressRender, 0, &Desc, sizeof(Desc));
	PX_ASSERTIF(pdesc == PX_NULL);

	pObject->diameter = radius * 2;
	return pObject;
}


PX_Object* PX_Object_RingProgressCreate(px_memorypool* mp, PX_Object* Parent, px_float x, px_float y, px_float radius, PX_FontModule* fm)
{
	PX_Object *pObject = PX_ObjectCreate(mp, Parent, x, y, 0, radius + radius, radius + radius, 0);
	if (!pObject)
	{
		return PX_NULL;
	}
	if (!PX_Object_RingProgressAttachObject(pObject, 0, 0, 0, radius, fm))
	{
		PX_ObjectDelete(pObject);
		return PX_NULL;
	}

	return pObject;
}

PX_Object_RingProgress* PX_Object_GetRingProgress(PX_Object* pObject)
{
	PX_Object_RingProgress* pdesc=(PX_Object_RingProgress*)PX_ObjectGetDescByType(pObject, PX_OBJECT_TYPE_RINGPROGRESS);
	PX_ASSERTIF(pdesc==PX_NULL);
	return pdesc;
	
}

px_void PX_Object_RingProgressSetValue(PX_Object* pObject, px_float value)
{
	PX_Object_RingProgress* pDesc = PX_Object_GetRingProgress(pObject);
	if (pDesc)
	{
		if (value<0)
		{
			value = 0;
		}
		if (value>pDesc->max)
		{
			value = pDesc->max;
		}
		pDesc->target_value = value;
	}
}


px_void PX_Object_RingProgressSetMaxValue(PX_Object* pObject, px_float value)
{
	PX_Object_RingProgress* pDesc = PX_Object_GetRingProgress(pObject);
	if (pDesc)
	{
		pDesc->max = value;
		
	}
}


px_void PX_Object_RingProgressSetCurrentValue(PX_Object* pObject, px_float value)
{
	PX_Object_RingProgress* pDesc = PX_Object_GetRingProgress(pObject);
	if (pDesc)
	{
		pDesc->current_value = value;
	}
}

px_float PX_Object_RingProgressGetValue(PX_Object* pObject)
{
	PX_Object_RingProgress* pDesc = PX_Object_GetRingProgress(pObject);
	if (pDesc)
	{
		return pDesc->target_value;
	}
	return 0;
}

px_void PX_Object_RingProgressSetSpeed(PX_Object* pObject, px_float value)
{
	PX_Object_RingProgress* pDesc = PX_Object_GetRingProgress(pObject);
	if (pDesc)
	{
		pDesc->Speed = value;
	}
}

