package com.ssafy.daoImpl;

import java.sql.*;
import java.util.ArrayList;

import com.ssafy.dao.AddressDao;
import com.ssafy.util.DBUtil;

public class AddressDaoImpl implements AddressDao {
	private static AddressDao addressDao = new AddressDaoImpl();
	private DBUtil dbUtil;

	private AddressDaoImpl() {
		dbUtil = DBUtil.getInstance();
	}

	public static AddressDao getAddressDao() {
		return addressDao;
	}

	@Override
	public ArrayList<String> getSidoList() throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		ArrayList<String> sidoList = new ArrayList<String>();
		try {
			conn = dbUtil.getConnection();
			String sql = "select distinct(city) from dongcode";
			pstmt = conn.prepareStatement(sql);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				String city = rs.getString("city");
				sidoList.add(city);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			dbUtil.close(rs, pstmt, conn);
		}
		return sidoList;
	}

	@Override
	public ArrayList<String> getGugunList(String sido) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		ArrayList<String> gugunList = new ArrayList<String>();
		try {
			conn = dbUtil.getConnection();
			String sql = "select distinct(gugun) from dongcode where city = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, sido);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				String city = rs.getString("gugun");
				gugunList.add(city);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			dbUtil.close(rs, pstmt, conn);
		}
		return gugunList;
	}

	@Override
	public ArrayList<String> getDongList(String sido, String gugun) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		ArrayList<String> dongList = new ArrayList<String>();
		try {
			conn = dbUtil.getConnection();
			String sql = "select distinct(dong) from dongcode where city = ? and gugun = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, sido);
			pstmt.setString(2, gugun);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				String city = rs.getString("dong");
				dongList.add(city);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			dbUtil.close(rs, pstmt, conn);
		}
		return dongList;
	}

}
