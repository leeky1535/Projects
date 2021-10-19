package com.ssafy.controller;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.*;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import service.AddressService;
import service.AddressServiceImpl;

/**
 * Servlet implementation class AddressService
 */
@WebServlet("/address")
public class AddressController extends HttpServlet {
	private static final long serialVersionUID = 1L;
	AddressService addressService = new AddressServiceImpl();

	public void init() {
		addressService = AddressServiceImpl.getAddressService();
	}

	public AddressController() {
		super();
		// TODO Auto-generated constructor stub
	}

	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		request.setCharacterEncoding("utf-8");
		String act = request.getParameter("act");
		System.out.println(act);
		if (act != null) {
			if (act.equals("sido-list")) {
				getSidoList(request, response);
			} else if (act.equals("gugun-list")) {
				getGugunList(request, response);
			} else if (act.equals("dong-list")) {
				getDongList(request, response);
			}
		}
	}

	private void getSidoList(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		response.setCharacterEncoding("utf-8");
		PrintWriter out = response.getWriter();
		ArrayList<String> sidoList = null;
		JSONArray arr = new JSONArray();
		try {
			sidoList = addressService.getSidoList();
			JSONObject obj = new JSONObject();
			obj.put("sido", sidoList);
			arr.add(obj);
		} catch (Exception e) {
			arr = new JSONArray();
			JSONObject obj = new JSONObject();
			obj.put("message_code", "-1");
			arr.add(obj);
			e.printStackTrace();
			e.printStackTrace();
			e.printStackTrace();
		} finally {
			out.print(arr.toJSONString());
			out.close();
		}
	}

	private void getGugunList(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		response.setCharacterEncoding("utf-8");
		String sido = request.getParameter("sido");
		PrintWriter out = response.getWriter();
		System.out.println(sido);
		ArrayList<String> gugunList = null;
		JSONArray arr = new JSONArray();
		try {
			gugunList = addressService.getGugunList(sido);
			JSONObject obj = new JSONObject();
			obj.put("gugun", gugunList);
			arr.add(obj);
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

	private void getDongList(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		response.setCharacterEncoding("utf-8");
		String sido = request.getParameter("sido");
		String gugun = request.getParameter("gugun");
		System.out.println(sido + " " + gugun);
		PrintWriter out = response.getWriter();
		ArrayList<String> dongList = null;
		JSONArray arr = new JSONArray();
		try {
			dongList = addressService.getDongList(sido, gugun);
			System.out.println(dongList.toString());
			JSONObject obj = new JSONObject();
			obj.put("dong", dongList);
			arr.add(obj);
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
