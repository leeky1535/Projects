package com.ssafy.controller;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.*;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import com.ssafy.dto.HouseDto;

import service.HouseService;
import service.HouseServiceImpl;

/**
 * Servlet implementation class HouseController
 */
@WebServlet("/house")
public class HouseController extends HttpServlet {
	private static final long serialVersionUID = 1L;
	HouseService houseService;

	public void init() {
		houseService = new HouseServiceImpl();
	}

	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		request.setCharacterEncoding("utf-8");
		String act = request.getParameter("act");
		if (act.equals("house-list")) {
			getHouseList(request, response);
		}
	}

	private void getHouseList(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		response.setCharacterEncoding("utf-8");
		String sido = request.getParameter("sido");
		String gugun = request.getParameter("gugun");
		String dong = request.getParameter("dong");
		String name = request.getParameter("name");
		String[] dongLatLng = null;
		ArrayList<HouseDto> houseList = null;
		PrintWriter out = response.getWriter();
		JSONArray arr = new JSONArray();
		try {
			dongLatLng = houseService.getDongLatLng(sido, gugun, dong);
			JSONObject obj0 = new JSONObject();
			obj0.put("dong-lat", dongLatLng[0]);
			obj0.put("dong-lng", dongLatLng[1]);
			houseList = houseService.getHouseList(sido, gugun, dong, name);
			System.out.println(houseList.size());
			ArrayList<JSONObject> arrJsonObj = new ArrayList<JSONObject>();
			for (int i = 0; i < houseList.size(); i++) {
				HouseDto house = houseList.get(i);
				JSONObject obj = new JSONObject();
				obj.put("dong-code", house.getDongCode());
				obj.put("sido", house.getSido());
				obj.put("gugun", house.getGugun());
				obj.put("dong", house.getDong());
				obj.put("apt-name", house.getAptName());
				obj.put("build-year", house.getBuildYear());
				obj.put("jibun", house.getJibun());
				obj.put("lat", house.getLat());
				obj.put("lng", house.getLng());
				arrJsonObj.add(obj);
			}
			obj0.put("house-list", arrJsonObj);
			arr.add(obj0);
		} catch (Exception e) {
			arr = new JSONArray();
			JSONObject obj = new JSONObject();
			obj.put("message_code", "-1");
			arr.add(obj);
			e.printStackTrace();
		} finally {
			out.print(arr.toJSONString());
			out.close();
		}

	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		// TODO Auto-generated method stub
		doGet(request, response);
	}

}