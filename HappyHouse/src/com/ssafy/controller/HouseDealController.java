package com.ssafy.controller;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.*;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import com.ssafy.dto.HouseDealDto;

import service.HouseDealService;
import service.HouseDealServiceImpl;

/**
 * Servlet implementation class HouseController
 */
@WebServlet("/housedeal")
public class HouseDealController extends HttpServlet {
	private static final long serialVersionUID = 1L;
	HouseDealService houseDealService;

	public void init() {
		houseDealService = new HouseDealServiceImpl();
	}

	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		request.setCharacterEncoding("utf-8");
		String act = request.getParameter("act");
		if (act.equals("house-deal-list")) {
			getHouseDealList(request, response);
		}
	}

	private void getHouseDealList(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		response.setCharacterEncoding("utf-8");
		String dongCode = request.getParameter("dong-code");
		String aptName = request.getParameter("apt-name");
		ArrayList<HouseDealDto> houseList = null;
		PrintWriter out = response.getWriter();
		JSONArray arr = new JSONArray();
		try {
			houseList = houseDealService.getHouseDeals(dongCode, aptName);
			System.out.println(houseList.size());
			for (int i = 0; i < houseList.size(); i++) {
				HouseDealDto house = houseList.get(i);
				System.out.println(house.getDealAmount());
				JSONObject obj = new JSONObject();
				obj.put("dong-code", house.getDongCode());
				obj.put("sido", house.getSido());
				obj.put("gugun", house.getGugun());
				obj.put("dong", house.getDong());
				obj.put("apt-name", house.getAptName());
				obj.put("deal-amount", house.getDealAmount());
				obj.put("build-year", house.getBuildYear());
				obj.put("deal-year", house.getDealYear());
				obj.put("deal-month", house.getDealMonth());
				obj.put("deal-day", house.getDealDay());
				obj.put("area", house.getArea());
				obj.put("floor", house.getFloor());
				obj.put("jibun", house.getJibun());
				obj.put("type", house.getType());
				obj.put("rent-money", house.getRentMoney());
				obj.put("lat", house.getLat());
				obj.put("lng", house.getLng());
				arr.add(obj);
			}
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
