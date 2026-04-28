void DocumentTut::addSelectedEdge(TopoDS_Edge& target)
{
	std::cout << "DocumentTut::addEdge "<<target.ShapeType()<<std::endl;
	
	if(BRep_Tool::IsGeometric(target))	
	{
		TopLoc_Location   L;
		Standard_Real     First;
		Standard_Real     Last;
		
		//2026/3/26 14:08:41 1 复制Edge 构造一个扫掠体  2 端点显示两个球体
		Handle(Geom_Curve) curve = BRep_Tool::Curve(target,L,First,Last);
		gp_Pnt P;
		gp_Vec V;
		curve->D1(First, P, V);
						
  		gp_Circ aCirc(gp_Ax2(P, gp_Dir(V)), 5.0);
		Handle(Geom_Circle) aCircCurve = new Geom_Circle(aCirc);
		BRepBuilderAPI_MakeEdge profile_circle_edgeMaker(aCircCurve);
		if(profile_circle_edgeMaker.IsDone())
		{
			BRepBuilderAPI_MakeWire profile_wireMaker(profile_circle_edgeMaker.Edge());
			BRepBuilderAPI_MakeWire path_wireMaker(target);
			if(profile_wireMaker.IsDone()&&path_wireMaker.IsDone())
			{
				Handle(AIS_InteractiveContext) aCtx = getContext_AIS_InteractiveContext();
				BRepOffsetAPI_MakePipeShell pipeShell(path_wireMaker.Wire());  // 创建扫掠对象，传入路径线
				pipeShell.SetDiscreteMode();
				pipeShell.Add(profile_wireMaker.Wire());                       // 添加截面轮廓
				pipeShell.Build();                                // 执行扫掠
				if(BRepBuilderAPI_PipeDone == pipeShell.GetStatus ())
				{	
					TopoDS_Shape result = pipeShell.Shape();          // 获取结果	
					Handle(AIS_ColoredShape) pipe = new AIS_ColoredShape(result);
					pipe->SetColor(Quantity_Color(Quantity_NOC_RED));
					aCtx->Display(pipe, 1, -1, Standard_False);	
					
					SELECTED.Add(target);
					std::cout<<	SELECTED.Size() <<std::endl;
					aCtx->UpdateCurrentViewer();
					return;	
			}	
		}	
	}	
			
	std::cerr << "process error\n"<<std::endl;	
}

Handle(AIS_Shape) DocumentTut::processLine(Handle(Geom_Curve)& curve,Standard_Real start,TopoDS_Edge& target)
{
	std::cout << "processLine"<<std::endl;
	//Handle(Geom_BezierCurve) line = Handle(Geom_BezierCurve)::DownCast(curve);		
	gp_Pnt P;
	gp_Vec V;
	curve->D1(start, P, V);
	std::cout<<"P(" << P.X() <<","<< P.Y() <<","<< P.Z() <<")"<<std::endl;
	std::cout<<"V(" << V.X() <<","<< V.Y() <<","<< V.Z() <<")"<<std::endl;	
	return Handle(AIS_Shape)();	
}
